#include "logreader.h"

#include <fcntl.h>
#include <unistd.h>
#include <iostream>

constexpr int BUFFER_SIZE = 1024;

LogReader::LogReader(const std::string &filepath)
{
    m_fd = open(filepath.c_str(), O_RDONLY);
    if (m_fd < 0)
    {
        throw std::runtime_error("failed to open file");
    }
}

LogReader::~LogReader()
{
    close(m_fd);
}

constexpr bool match(std::string_view input, LogLevel level) noexcept {
    std::string lowerInput{input};
    
    for (auto& ch : lowerInput) {
        if (ch >= 'A' && ch <= 'Z')
        {
            ch = static_cast<char>(ch + 32);
        }
    }

    switch (level)
    {
    case LogLevel::DEBUG:
        return lowerInput.find("debug") != std::string::npos;
    case LogLevel::INFO:
        return lowerInput.find("info") != std::string::npos;
    case LogLevel::WARN:
        return lowerInput.find("warn") != std::string::npos;
    case LogLevel::ERROR:
        return lowerInput.find("error") != std::string::npos;
    case LogLevel::FATAL:
        return lowerInput.find("fatal") != std::string::npos;
    }
    
    return false;
}

std::optional<std::vector<std::pair<std::string, LogLevel>>> LogReader::get_last_logs()
{
    char buf[BUFFER_SIZE] = {0};
    std::string line = "";
    std::vector<std::pair<std::string, LogLevel>> lines;
    while (read(m_fd, buf, BUFFER_SIZE))
    {
        for (int i = 0; i < BUFFER_SIZE; i++)
        {
            if (buf[i] == '\0')
            {
                if (match(line, LogLevel::DEBUG))
                {
                    lines.push_back(std::make_pair(line, LogLevel::DEBUG));
                }
                else if (match(line, LogLevel::INFO))
                {
                    lines.push_back(std::make_pair(line, LogLevel::INFO));
                }
                else if (match(line, LogLevel::WARN))
                {
                    lines.push_back(std::make_pair(line, LogLevel::WARN));
                }
                else if (match(line, LogLevel::ERROR))
                {
                    lines.push_back(std::make_pair(line, LogLevel::ERROR));
                }
                else if (match(line, LogLevel::FATAL))
                {
                    lines.push_back(std::make_pair(line, LogLevel::FATAL));
                }
                break;
            }

            if (buf[i] == '\n')
            {
                if (match(line, LogLevel::DEBUG))
                {
                    lines.push_back(std::make_pair(line, LogLevel::DEBUG));
                }
                else if (match(line, LogLevel::INFO))
                {
                    lines.push_back(std::make_pair(line, LogLevel::INFO));
                }
                else if (match(line, LogLevel::WARN))
                {
                    lines.push_back(std::make_pair(line, LogLevel::WARN));
                }
                else if (match(line, LogLevel::ERROR))
                {
                    lines.push_back(std::make_pair(line, LogLevel::ERROR));
                }
                else if (match(line, LogLevel::FATAL))
                {
                    lines.push_back(std::make_pair(line, LogLevel::FATAL));
                }
                line = "";
                continue;
            }

            line += buf[i];
        }
    }
    
    if (lines.empty())
    {
        return std::nullopt;
    }

    return lines;
}
