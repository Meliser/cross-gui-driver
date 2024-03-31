#include <iostream>
#include </usr/local/include/httpserver.hpp>
#include <memory>
#include <mutex>
#include <nlohmann/json.hpp>
#include <boost/dll/import.hpp>
#include <boost/dll/alias.hpp>
#include "endpoint_handler_i.h"



using namespace httpserver;
class hello_world_res : public http_resource {
public:
    hello_world_res(std::unique_ptr<EndpointHandlerI> handler) : handler(std::move(handler))
    {}
    std::shared_ptr<http_response> render_POST(const http_request& req) {
        using json = nlohmann::json;
        
        std::string_view content = req.get_content();
        json data = json::parse(content);
        const std::string& name = data["elementName"];
        //std::cout << "GOT " << data << std::endl;
        return std::shared_ptr<http_response>(new string_response(handler->getText(name)));
    }
private:
    std::unique_ptr<EndpointHandlerI> handler;
};

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
        DriverApp() : m_ws(create_webserver().port(8080))
        {
            std::cout << "Construct DriverApp" << std::endl;
            m_plugin_handle = getEndpointHandler("qt");
            std::unique_ptr<EndpointHandlerI> h = m_plugin_handle();
            h->init();
            m_hwr = std::make_unique<hello_world_res>(std::move(h));
            m_ws.register_resource("/hello", m_hwr.get());
            m_ws.start(false);
        }
        ~DriverApp()
        {
            std::cout << "Destruct DriverApp" << std::endl;
        }
    private:
        std::function<HandlerType> m_plugin_handle;
        webserver m_ws;
        std::unique_ptr<hello_world_res> m_hwr;
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