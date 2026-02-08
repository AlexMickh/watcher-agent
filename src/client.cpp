#include "client.h"

Client::Client(std::shared_ptr<grpc::Channel> channel, std::unique_ptr<ILogReader> logreader, bool &done)
    : m_stub(Watcher::NewStub(channel)), m_logreader(std::move(logreader))
{
    m_thread = std::thread([this, &done]
    {
        while (!done)
        {
            std::optional<std::vector<std::pair<std::string, LogLevel>>> data = m_logreader->get_last_logs();
            if (data.has_value())
            {
                send_logs(std::move(*data));
            }
        }
    });
}

Client::~Client()
{
    m_thread.join();
}

void Client::send_logs(std::vector<std::pair<std::string, LogLevel>> &&data)
{
    api::v1::GetLogsRequest req;
    google::protobuf::RepeatedPtrField<api::v1::LogItem> *logs = req.mutable_logs();

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

        grpc::Status status = m_stub->GetLogs(&ctx, req, &resp);
        if (status.ok())
        {
            std::cout << "ok" << std::endl;
        }
        else 
        {
            std::cout << status.error_message() << std::endl;
        }
    }
}
