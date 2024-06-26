#include <iostream>
#include </usr/local/include/httpserver.hpp>
#include <memory>
#include <mutex>
#include <nlohmann/json.hpp>
#include <boost/dll/import.hpp>
#include <boost/dll/alias.hpp>

#include <string_view>

#include "endpoints/session.h"
#include "endpoints/title.h"
#include "endpoints/window_rect.h"

using HandlerType = std::unique_ptr<EndpointHandlerI>(void*);
std::function<HandlerType> getEndpointHandler(const std::string& type)
{
    if ("qt" == type) {
        std::function<HandlerType> creator = boost::dll::import_alias<HandlerType>(
            "src/drivers/qt/qt-driver",
            "create_handler",
            boost::dll::load_mode::append_decorations);
        return creator;
    }
    else if ("java" == type) {
        std::function<HandlerType> creator = boost::dll::import_alias<HandlerType>(
            "/home/kds/src/private/cross-gui-driver/build/src/drivers/java/adapter/java-driver-adapter",
            "create_handler",
            boost::dll::load_mode::append_decorations);
        return creator;
    }
    return {};
    //return std::make_unique<QtEndpointHandler>();
}

namespace
{
    class DriverApp
    {
    public:
        DriverApp(const char* name, void* config) : m_ws(httpserver::create_webserver().port(8080))
        {
            std::cout << "Construct DriverApp" << std::endl;

            m_plugin_handle = getEndpointHandler(name);
            std::shared_ptr<EndpointHandlerI> h = m_plugin_handle(config);

            m_session_h = std::make_unique<session>(h);
            m_ws.register_resource("/session", m_session_h.get());
            m_title_h = std::make_unique<title>(h);
            m_ws.register_resource("/session/{session_id}/window/{window_id}/title", m_title_h.get());
            m_window_rect_h = std::make_unique<window_rect>(h);
            m_ws.register_resource("/session/{session_id}/window/{window_id}/rect", m_window_rect_h.get());

            m_ws.start(false);
        }
        ~DriverApp()
        {
            std::cout << "Destruct DriverApp" << std::endl;
        }
    private:
        std::function<HandlerType> m_plugin_handle;
        httpserver::webserver m_ws;
        std::unique_ptr<session> m_session_h;
        std::unique_ptr<title> m_title_h;
        std::unique_ptr<window_rect> m_window_rect_h;
    };

    std::unique_ptr<DriverApp> global_app;
}

// todo kds: hidden
__attribute__ ((visibility ("default"))) bool init(const char* name, void* config)
{
    std::cout << "Driver init for " << name << std::endl;
    global_app.reset(new DriverApp(name, config));
    return true;
}

void deinit()
{
    global_app.reset();
}

BOOST_DLL_ALIAS(
    init,
    init_driver
)

// int main(int argc, char** argv) {
//     std::call_once(inited, init);
//     return 0;
// }