#pragma once
#include <string>

class EndpointHandlerI
{
public:
    virtual ~EndpointHandlerI() = default;
    virtual bool init() { return true; }
    virtual std::string getText(const std::string& name) = 0;
};
