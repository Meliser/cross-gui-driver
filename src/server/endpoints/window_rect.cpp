
#include "window_rect.h"
#include "endpoint_handler_i.h"

#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string_view>

std::shared_ptr<httpserver::http_response> window_rect::render_GET(const httpserver::http_request& req) {
    using json = nlohmann::json;
    const std::vector<std::string> pieces = req.get_path_pieces();
    assert(pieces.size() == 5);
    assert(pieces[0] == "session");
    assert(pieces[2] == "window");
    assert(pieces[4] == "rect");

    const std::string& session_id = pieces[1];
    const std::string& window_id = pieces[3];

    std::optional<WidgetRect> rect = handler->getWidgetRect(window_id);
    if (rect.has_value()) {
        json o;
        json data;
        data["height"] = rect->height;
        data["width"] = rect->width;
        data["x"] = rect->x;
        data["y"] = rect->y;
        o["value"] = std::move(data);
        return std::shared_ptr<httpserver::http_response>(
            new httpserver::string_response(o.dump(),
             httpserver::http::http_utils::http_ok,
             "application/json; charset=utf-8"));
    }
    else {
        return std::shared_ptr<httpserver::http_response>(new httpserver::string_response("BAD", httpserver::http::http_utils::http_not_found));
    }
}