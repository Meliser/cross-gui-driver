#include <iostream>
#include </usr/local/include/httpserver.hpp>
#include <memory>
#include <mutex>
#include <nlohmann/json.hpp>
#include <boost/dll/import.hpp>
#include <boost/dll/alias.hpp>

#include <string_view>

#include "endpoints/session.h"

using HandlerType = std::unique_ptr<EndpointHandlerI>();
std::function<HandlerType> getEndpointHandler(const std::string& type)
{
    if ("qt" == type) {
        // load qt-driver.so
        // find std::unique_ptr<EndpointHandlerI> create() func
        std::function<HandlerType> creator = boost::dll::import_alias<HandlerType>(
            "src/drivers/qt/qt-driver",
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
        DriverApp() : m_ws(httpserver::create_webserver().port(8080))
        {
            std::cout << "Construct DriverApp" << std::endl;

            m_plugin_handle = getEndpointHandler("qt");
            std::shared_ptr<EndpointHandlerI> h = m_plugin_handle();
            h->init();

            m_session_h = std::make_unique<session>(h);
            m_ws.register_resource("/session", m_session_h.get());
            
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
    };

    std::unique_ptr<DriverApp> global_app;
}

bool init()
{
    std::cout << "Driver init" << std::endl;
    global_app.reset(new DriverApp);
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