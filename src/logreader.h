#pragma once

#include "ilogreader.h"

class LogReader : public ILogReader
{
public:
    LogReader(const std::string &filepath);
    ~LogReader();

    virtual std::optional<std::vector<std::pair<std::string, LogLevel>>> get_last_logs() override;

private:
    int m_fd;
};