#pragma once

#include "endpoint_handler_i.h"

#include </usr/local/include/httpserver.hpp>

class session : public httpserver::http_resource {
public:
    session(std::shared_ptr<EndpointHandlerI> handler) : handler(std::move(handler))
    {}
    std::shared_ptr<httpserver::http_response> render_POST(const httpserver::http_request& req);
private:
    std::shared_ptr<EndpointHandlerI> handler;
};