#pragma once

class ICpu
{
public:
    virtual ~ICpu() = default;
    virtual double calculate_usage() = 0;
};