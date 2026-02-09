#include <iostream>
#include "config.h"
#include "logreader.h"
#include <memory>
#include "client.h"
#include <csignal>
#include "cpu.h"
#include "retryer.h"

bool done = false;

void signalHandler(int sig)
{
    done = true;
    exit(sig);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "need config path" << std::endl;
        exit(1);
    }

    std::signal(SIGINT, signalHandler);

    Config cfg{argv[1]};

    std::unique_ptr<ILogReader> reader = std::make_unique<LogReader>(cfg.get_log_path());
    std::unique_ptr<ICpu> cpu = std::make_unique<Cpu>();

    Client client{
        grpc::CreateChannel(cfg.get_server_addr(), grpc::InsecureChannelCredentials()),
        std::move(reader), std::move(cpu), std::move(cfg.get_service_name())};

    if (!client.start(done))
    {
        Retryer<Client> retryer{client};
        retryer.retry(5000);
        client.start(done);
    }

    return 0;
}
