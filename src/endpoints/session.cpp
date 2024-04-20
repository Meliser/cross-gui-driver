
#include "session.h"

#include <nlohmann/json.hpp>
#include <string_view>

std::shared_ptr<httpserver::http_response> session::render_POST(const httpserver::http_request& req) {
    using json = nlohmann::json;
    
    std::string_view content = req.get_content();
    json data = json::parse(content);
    const std::string& name = data["elementName"];
    //std::cout << "GOT " << data << std::endl;
    return std::shared_ptr<httpserver::http_response>(new httpserver::string_response(handler->getText(name)));
}