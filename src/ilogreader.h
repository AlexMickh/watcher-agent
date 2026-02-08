#pragma once

#include <optional>
#include <vector>
#include <string>

enum class LogLevel 
{
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
};

class ILogReader
{
public:
    virtual ~ILogReader() = default;
    virtual std::optional<std::vector<std::pair<std::string, LogLevel>>> get_last_logs() = 0;
};