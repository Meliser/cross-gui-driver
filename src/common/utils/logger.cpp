#include "logger.h"

#include <iostream>

Logger::Logger()
{
    m_prefix = "CGD";
}

void Logger::debug(const std::string& msg)
{
    std::cout << m_prefix << " DEBUG " << msg << std::endl;
}