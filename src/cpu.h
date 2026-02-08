#pragma once

#include "icpu.h"

struct CpuTimes
{
    unsigned long long user;
    unsigned long long nice;
    unsigned long long system;
    unsigned long long idle;
    unsigned long long iowait;
    unsigned long long irq;
    unsigned long long softirq;
};

class Cpu : public ICpu
{
public:
    Cpu();

    virtual double calculate_usage() override;
private:
    inline void clear()
    {
        m_t1 = {0};
        m_t2 = {0};
    }

    void read_data();

    CpuTimes m_t1;
    CpuTimes m_t2;
};