#include "client.h"

Client::Client(std::shared_ptr<grpc::Channel> channel, std::unique_ptr<ILogReader> logreader, std::unique_ptr<ICpu> cpu,
               const std::string &service_name)
    : m_stub(Watcher::NewStub(channel)), m_logreader(std::move(logreader)), m_cpu(std::move(cpu)), m_service_name(std::move(service_name))
{
    
}

Client::~Client()
{
    m_log_thread.join();
    m_cpu_thread.join();
}

bool Client::start(bool &done)
{
    if (!handshake())
    {
        return false;
    }

    m_log_thread = std::thread([this, &done]
    {
        std::optional<std::vector<std::pair<std::string, LogLevel>>> last_logs = std::nullopt;

        while (!done)
        {
            if (last_logs.has_value())
            {
                send_logs(last_logs.value());
                last_logs = std::nullopt;
                continue;
            }

            std::optional<std::vector<std::pair<std::string, LogLevel>>> data = m_logreader->get_last_logs();
            if (data.has_value())
            {
                if (!send_logs(data.value()))
                {
                    last_logs = data;
                }
            }
        }
    });

    m_cpu_thread = std::thread([this, &done]{
        send_cpu_usage(done);
    });

    return true;
}

bool Client::send_logs(const std::vector<std::pair<std::string, LogLevel>> &data)
{
    api::v1::GetLogsRequest req;
    google::protobuf::RepeatedPtrField<api::v1::LogItem> *logs = req.mutable_logs();

    bool is_success = true;

    for (auto &el : data)
    {
        api::v1::LogItem log;
        log.set_log(el.first);
        switch (el.second)
        {
        case LogLevel::DEBUG:
            log.set_log_type(api::v1::DEBUG);
        case LogLevel::INFO:
            log.set_log_type(api::v1::INFO);
        case LogLevel::WARN:
            log.set_log_type(api::v1::WARN);
        case LogLevel::ERROR:
            log.set_log_type(api::v1::ERROR);
        case LogLevel::FATAL:
            log.set_log_type(api::v1::FATAL);
        }

        logs->Add(std::move(log));

        grpc::ClientContext ctx;
        google::protobuf::Empty resp;

        ctx.AddMetadata("service_name", m_service_name);

        grpc::Status status = m_stub->GetLogs(&ctx, req, &resp);

        is_success = status.ok();
    }

    return is_success;
}

void Client::send_cpu_usage(bool &done)
{
    grpc::ClientContext ctx;
    google::protobuf::Empty resp;

    ctx.AddMetadata("service_name", m_service_name);

    std::unique_ptr<grpc::ClientWriter<api::v1::CpuUsageRequest>> writer(m_stub->CpuUsage(&ctx, &resp));

    while (!done)
    {
        api::v1::CpuUsageRequest req;
        req.set_usage(m_cpu->calculate_usage());
        if (!writer->Write(req))
        {
            continue;
        }
    }
    
    writer->WritesDone();
    writer->Finish();
}
