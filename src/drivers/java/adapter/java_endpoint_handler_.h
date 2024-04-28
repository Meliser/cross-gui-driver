#pragma once
#include "endpoint_handler_i.h"
#include <memory>
#include <thread>
#include <future>
#include <variant>

#include <jni.h>

#include <boost/dll/alias.hpp>


class JavaEndpointHandler : public EndpointHandlerI
{
    using AutoDetachT = std::unique_ptr<JNIEnv, std::function<void(JNIEnv*)>>;
public:
    JavaEndpointHandler(void* config);
    std::string getText(const std::string& name) override;
    std::string getTitle(const std::string& name) override;
    void nameAll() override;
    std::optional<WidgetRect> getWidgetRect(const std::string& name) override;
private:
    AutoDetachT get_env();
private:
    JavaVM* m_vm;
};

std::unique_ptr<EndpointHandlerI> create(void* config);

BOOST_DLL_ALIAS(
    create,
    create_handler
)