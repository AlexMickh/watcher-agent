#pragma once

#include <concepts>
#include <type_traits>

template <class T>
concept HasRetry = requires(T t) {
    { t.retry() } -> std::same_as<bool>;
};

template <HasRetry T>
class Retryer
{
public:
    Retryer(T &object)
        : m_object(object)
    {}

    inline void retry(int delay_ms)
    {
        bool is_complete = false;
        while (!is_complete)
        {
            is_complete = m_object.retry();
            if (!is_complete)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
            }
        }
    }
private:
    T &m_object;
};
