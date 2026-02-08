#pragma once

#include <grpc++/grpc++.h>
#include <protos/api.grpc.pb.h>
#include "ilogreader.h"
#include <thread>
#include "icpu.h"

using api::v1::Watcher;

class Client
{
public:
    Client(std::shared_ptr<grpc::Channel> channel, std::unique_ptr<ILogReader> logreader, bool &done, std::unique_ptr<ICpu> cpu);
    ~Client();
private:
    void send_logs(std::vector<std::pair<std::string, LogLevel>> &&data);
    void send_cpu_usage(bool &done);

    std::unique_ptr<Watcher::Stub> m_stub;
    std::unique_ptr<ILogReader> m_logreader;
    std::thread m_log_thread;
    std::unique_ptr<ICpu> m_cpu;
    std::thread m_cpu_thread;
};