#pragma once

#include <jni.h>
#include <string_view>

template <typename Call>
void callJni(JNIEnv* env, Call c)
{
    c();
}

class Impl
{
public:
    Impl(JNIEnv* env);
    Impl() : m_obj{nullptr}, m_env{nullptr} {};
    jobject getObj() const { return m_obj; };
protected:
    jobject m_obj;
    JNIEnv* m_env;
};

namespace java
{
    namespace lang
    {
        class Object : public Impl
        {
        public:
            Object(JNIEnv* env);
        protected:
            Object() {};
        };

        class String : public Object
        {
        public:
            String(JNIEnv* env);
            bool endsWith(String suffix);
        };

        class ClassLoader : public Object
        {
        public:
            ClassLoader(JNIEnv* env);
            static ClassLoader getSystemClassLoader(JNIEnv* env);
        private:
            ClassLoader(JNIEnv* env, jobject loader);
            static constexpr std::string_view class_name = "java/lang/ClassLoader";
        };

        // void foo(JNIEnv* env)
        // {
        //     callJni(env, &ClassLoader::getSystemClassLoader);
        // }
    }

}

void defineClass(JNIEnv* env, const java::lang::ClassLoader& loader);

namespace kds
{
    class Controller
    {
    public:
        //Controller();
        static std::string getText(JNIEnv* env, const std::string& name);
        static void walkTree(JNIEnv* env);
    private:
        static constexpr std::string_view class_name = "org/kds/Controller";
    };
}

