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
    Client(std::shared_ptr<grpc::Channel> channel, std::unique_ptr<ILogReader> logreader, std::unique_ptr<ICpu> cpu,
           const std::string &service_name);
    ~Client();

    bool start(bool &done);

    inline bool retry()
    {
        return handshake();
    }

private:
    bool send_logs(const std::vector<std::pair<std::string, LogLevel>> &data);
    void send_cpu_usage(bool &done);
    
    inline bool handshake()
    {
        api::v1::HandShakeRequest req;
        req.set_service_name(m_service_name);

        grpc::ClientContext ctx;
        google::protobuf::Empty resp;
        grpc::Status status = m_stub->HandShake(&ctx, req, &resp);

        return status.ok();
    }

    std::unique_ptr<Watcher::Stub> m_stub;
    std::unique_ptr<ILogReader> m_logreader;
    std::thread m_log_thread;
    std::unique_ptr<ICpu> m_cpu;
    std::thread m_cpu_thread;
    std::string m_service_name;
};