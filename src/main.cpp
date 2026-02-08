#include <iostream>
#include "config.h"
#include "logreader.h"
#include <memory>
#include "client.h"
#include <csignal>

bool done = false;

void signalHandler(int)
{
    done = true;
}

int main (int argc, char *argv[]) {
    if (argc != 2)
    {
        std::cerr << "need config path" << std::endl;
        exit(1);
    }

    std::signal(SIGINT, signalHandler);

    Config cfg{argv[1]};

    std::unique_ptr<ILogReader> reader = std::make_unique<LogReader>(cfg.get_log_path());
    
    Client client{
        grpc::CreateChannel(cfg.get_server_addr(), grpc::InsecureChannelCredentials()),
        std::move(reader),
        done
    };

    return 0;
}
