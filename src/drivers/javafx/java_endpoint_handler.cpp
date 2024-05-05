#include "java_endpoint_handler_.h"
#include "jni_md.h"
#include <chrono>
#include <iostream>
#include <memory>
#include <exception>
#include <fstream>
#include <sstream>
#include "bridge.h"
#include "logger.h"

std::unique_ptr<EndpointHandlerI> create(void *config) {
  return std::make_unique<JavaEndpointHandler>(config);
}

JavaEndpointHandler::AutoDetachT JavaEndpointHandler::get_env()
{
    JNIEnv* env = nullptr;
    int res = m_vm->GetEnv((void**)&env, JNI_VERSION_1_6);
    if (JNI_EDETACHED == res) {
        std::cout << "GetEnv: not attached" << std::endl;
        if (JNI_OK != m_vm->AttachCurrentThread((void**)&env, NULL)) {
            std::cout << "Failed to attach" << std::endl;
            return {};
        }
    }
    else if (res == JNI_EVERSION) {
        std::cout << "GetEnv: version not supported" << std::endl;
        return {};
    }

    std::cout << "Attach OK" << std::endl;
    assert(env);

    // todo kds: deleter reqs, can ptr be null?
    auto detach_th = [this] (JNIEnv* env) {
        if (!env) {
            std::cout << "WARNING!! env NULL. Ignore" << std::endl;
            return;
        }
        std::cout << "Detach thread" << std::endl;
        if (env->ExceptionCheck()) {
            env->ExceptionDescribe();
        }
        m_vm->DetachCurrentThread();
    };
    return {env, detach_th};
}

static jobject aa;

JavaEndpointHandler::JavaEndpointHandler(void* config)
{
    LOG_DEBUG("HELLLLLOOOOO !!!!");
    std::cout << "JavaEndpointHandler" << std::endl;
    struct JavaConf{
        JNIEnv* env;
    };
    JavaConf* conf = static_cast<JavaConf*>(config);// todo: strict aliasing
    if (0 != conf->env->GetJavaVM(&m_vm)) {
        throw std::runtime_error("Failed to get Java VM");
    }

    JNIEnv* env = conf->env;

    java::lang::ClassLoader loader = java::lang::ClassLoader::getSystemClassLoader(env);
    defineClass(env, loader);
}


std::string JavaEndpointHandler::getText(const std::string& name)
{
    //nameAll();
    LOG_DEBUG("java !!!!");

    std::cout << "java get text th" << std::this_thread::get_id() <<std::endl;
    JavaEndpointHandler::AutoDetachT env = get_env();

    if (!env)
        return "NO ENV";
    // nameAll();
    // return "DDDD";

    return kds::Controller::getText(env.get(), name);
}

std::string JavaEndpointHandler::getTitle(const std::string& name)
{
    return "AAA";
}

std::optional<WidgetRect> JavaEndpointHandler::getWidgetRect(const std::string& name)
{
    std::optional<WidgetRect> opt_rect;
    return opt_rect;
}

void JavaEndpointHandler::nameAll()
{
    JavaEndpointHandler::AutoDetachT env = get_env();
    kds::Controller::walkTree(env.get());
}