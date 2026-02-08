#pragma once

#include <string>

class Config
{
public:
    Config(const std::string &filepath);
    Config(const Config &) = delete;
    void operator=(const Config &) = delete;

    const std::string &get_service_name() const;
    const std::string &get_server_addr() const;
    const std::string &get_log_path() const;

private:
    std::string m_service_name;
    std::string m_server_addr;
    std::string m_log_path;
};
