#include "config.h"
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

Config::Config(const std::string &filepath)
{
    std::ifstream file{filepath};
    if (!file.is_open())
    {
        throw std::runtime_error("failed to open file");
    }

    json data = json::parse(file);

    m_service_name = data["service_name"];
    m_server_addr = data["server_addr"];
    m_log_path = data["log_path"];
}

const std::string &Config::get_service_name() const
{
    return m_service_name;
}

const std::string &Config::get_server_addr() const
{
    return m_server_addr;
}

const std::string &Config::get_log_path() const
{
    return m_log_path;
}
