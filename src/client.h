#pragma once

#include <grpc++/grpc++.h>
#include <protos/api.grpc.pb.h>
#include "ilogreader.h"
#include <thread>

using api::v1::Watcher;

class Client
{
public:
    Client(std::shared_ptr<grpc::Channel> channel, std::unique_ptr<ILogReader> logreader, bool &done);
    ~Client();
private:
    void send_logs(std::vector<std::pair<std::string, LogLevel>> &&data);

    std::unique_ptr<Watcher::Stub> m_stub;
    std::unique_ptr<ILogReader> m_logreader;
    std::thread m_thread;
};