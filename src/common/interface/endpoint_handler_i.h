#pragma once
#include <string>
#include <optional>

struct WidgetRect
{
    int x;
    int y;
    int width;
    int height;
};

class EndpointHandlerI
{
public:
    virtual ~EndpointHandlerI() = default;
    virtual std::string getText(const std::string& name) = 0;
    virtual std::string getTitle(const std::string& name) = 0;
    virtual std::optional<WidgetRect> getWidgetRect(const std::string& name) = 0;
    virtual void nameAll() = 0;
};
