
#include "title.h"

#include <iostream>
#include <nlohmann/json.hpp>
#include <string_view>

std::shared_ptr<httpserver::http_response> title::render_GET(const httpserver::http_request& req) {
    using json = nlohmann::json;
    const std::vector<std::string> pieces = req.get_path_pieces();
    assert(pieces.size() == 5);
    assert(pieces[0] == "session");
    assert(pieces[2] == "window");
    assert(pieces[4] == "title");

    const std::string& session_id = pieces[1];
    const std::string& window_id = pieces[3];

    return std::shared_ptr<httpserver::http_response>(new httpserver::string_response(handler->getTitle(window_id)));
}