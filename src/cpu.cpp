#include "cpu.h"
#include <fstream>
#include <chrono>
#include <thread>

Cpu::Cpu()
{
    clear();
}

double Cpu::calculate_usage()
{
    read_data();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    read_data();

    unsigned long long total_time1 = m_t1.user + m_t1.nice + m_t1.system + m_t1.idle + m_t1.iowait + m_t1.irq + m_t1.softirq;
    unsigned long long total_time2 = m_t2.user + m_t2.nice + m_t2.system + m_t2.idle + m_t2.iowait + m_t2.irq + m_t2.softirq;

    // Idle time for m_t1 and m_t2
    unsigned long long idle_time1 = m_t1.idle + m_t1.iowait;
    unsigned long long idle_time2 = m_t2.idle + m_t2.iowait;

    // Difference in total time and idle time over the interval
    unsigned long long total_diff = total_time2 - total_time1;
    unsigned long long idle_diff = idle_time2 - idle_time1;

    clear();

    if (total_diff == 0)
    {
        return 0.0;
    }

    // CPU usage is (Total Time Diff - Idle Time Diff) / Total Time Diff
    return static_cast<double>(total_diff - idle_diff) / total_diff * 100.0;
}

void read_data(CpuTimes &t)
{
    std::ifstream file{"/proc/stat"};
    if (!file.is_open())
    {
        throw std::runtime_error("failed to open /proc/stat");
    }

    std::string line;
    std::getline(file, line);
    file.close();

    std::sscanf(line.c_str(), "cpu  %llu %llu %llu %llu %llu %llu %llu",
                &t.user, &t.nice, &t.system, &t.idle, &t.iowait, &t.irq, &t.softirq);
}

void Cpu::read_data()
{
    if (m_t1.user = 0)
    {
        ::read_data(m_t1);
    }
    else
    {
        ::read_data(m_t2);
    }
}
