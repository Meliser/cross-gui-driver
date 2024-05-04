#include "bridge.h"
#include "elf_parser.h"

#include <dlfcn.h>

#include <jni.h>
#include <iostream>
#include <fstream>
#include <sstream>

namespace
{
    jclass findClass(JNIEnv* env, std::string_view path)
    {
        jclass cls = env->FindClass(path.data());
        if (!cls) {
            if (env->ExceptionCheck()) {
                env->ExceptionDescribe();
                env->ExceptionClear();
            }
            throw std::runtime_error{std::string{"Class "} + path.data() + "was not found"};
        }
        return cls;
    }

    jmethodID getStaticMethodId(JNIEnv* env, jclass cls, std::string_view name, std::string_view sig)
    {
        jmethodID m_id = env->GetStaticMethodID(cls, name.data(), sig.data());
        if (!m_id) {
            if (env->ExceptionCheck()) {
                env->ExceptionDescribe();
                env->ExceptionClear();
            }
            throw std::runtime_error{std::string{"Static method "} + name.data() + " of class was not found"};
        }
        return m_id;
    }

    jmethodID getMethodId(JNIEnv* env, jclass cls, std::string_view name, std::string_view sig)
    {
        jmethodID m_id = env->GetMethodID(cls, name.data(), sig.data());
        if (!m_id) {
            if (env->ExceptionCheck()) {
                env->ExceptionDescribe();
                env->ExceptionClear();
            }
            throw std::runtime_error{std::string{"Method "} + name.data() + " of class was not found"};
        }
        return m_id;
    }

    jobject getNewObject(JNIEnv* env, jclass cls, jmethodID constructor_id)
    {
        jobject obj = env->NewObject(cls, constructor_id);
        if (!obj) {
            if (env->ExceptionCheck()) {
                env->ExceptionDescribe();
                env->ExceptionClear();
            }
            throw std::runtime_error{std::string{"Failed to construct class"}};
        }
        return obj;
    }
}

Impl::Impl(JNIEnv* env) : m_env(env)
{

}

namespace java {
namespace lang {

Object::Object(JNIEnv* env) : Impl(env)
{

}

ClassLoader::ClassLoader(JNIEnv* env)
{
    jclass cls = findClass(env, class_name);
    jmethodID constructor_id = getMethodId(env, cls,"<init>", "()V");
    m_obj = getNewObject(env, cls, constructor_id); 
    m_env = env;
}

ClassLoader::ClassLoader(JNIEnv* env, jobject obj)
{
    m_env = env;
    m_obj = obj;
}

ClassLoader ClassLoader::getSystemClassLoader(JNIEnv* env) {
    jclass cls = findClass(env, class_name);
    jmethodID m_id = getStaticMethodId(env, cls, "getSystemClassLoader", "()Ljava/lang/ClassLoader;");

    jobject loader_obj = env->CallStaticObjectMethod(cls, m_id);
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
        throw std::runtime_error{std::string{"Failed to call getSystemClassLoader"}};
    }

    return ClassLoader{env, loader_obj};
}

} // namespace lang
} // namespace java

namespace kds
{
std::string Controller::getText(JNIEnv* env, const std::string& name)
{
    jclass cls = findClass(env, class_name);
    jmethodID m_id = getStaticMethodId(env, cls, "getText", "(Ljava/lang/String;)Ljava/lang/String;");

    jstring jname = env->NewStringUTF(name.c_str());
    if (!jname || env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
        throw std::runtime_error{std::string{"Failed to create string"}};
    }

    jobject obj = env->CallStaticObjectMethod(cls, m_id, jname);
    if (!obj || env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
        throw std::runtime_error{std::string{"Failed to call getText"}};
    }
    const char* raw = env->GetStringUTFChars((jstring)obj, NULL);
    if (!raw || env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
        throw std::runtime_error{std::string{"Failed to call GetStringUTFChars"}};
    }
    return std::string{raw};
}

void Controller::walkTree(JNIEnv* env)
{
    jclass cls = findClass(env, class_name);
    jmethodID m_id = getStaticMethodId(env, cls, "walkTree", "()V");
    env->CallStaticVoidMethod(cls, m_id);
    if (env->ExceptionCheck()) {
        std::cout << "BADDDD" << std::endl;
        env->ExceptionDescribe();
        env->ExceptionClear();
        throw std::runtime_error{std::string{"Failed to call walkTree"}};
    }
}
}

void defineClass(JNIEnv* env, const java::lang::ClassLoader& loader)
{
    Dl_info info = {0};
    if (!dladdr((void*)defineClass, &info)) {
        std::cout << "BAD I DONT KNOW" << std::endl;
        throw std::runtime_error{"Failed to get dladdr"};
    }
    // https://stackoverflow.com/questions/43409167/find-location-of-loaded-shared-library-from-in-that-shared-library

    std::cout << "OUR LIB is " << info.dli_fname << std::endl;


    ElfParser parser{info.dli_fname};
    ElfParser::SectionDesc desc = parser.getSectionContent(".cgd-javafx-driver-adapter");
    jclass ctl = env->DefineClass("org/kds/Controller", loader.getObj(), (const jbyte*)desc.content, desc.size);
}