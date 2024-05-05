#pragma once

#include <string>

enum class Options
{
    TIME=0x1,
    TH=0x2,
    PROJ=0x4
};

class Logger
{
public:
    template <typename ...Args>
    static Logger& getInstance(Args...);
    void debug(const std::string& msg);
private:
    Logger();
private:
    std::string m_prefix;
};

template <typename ...Args>
Logger& Logger::getInstance(Args... args)
{
    static Logger l(args...);
    return l;
}

#ifdef NDEBUG
    #define LOG_DEBUG(msg)
#else
    #define LOG_DEBUG(msg) Logger::getInstance().debug(msg)
#endif