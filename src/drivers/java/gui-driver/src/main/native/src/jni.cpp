#include "org_kds_Plugin.h"
#include <stdio.h>
#include <iostream>

bool init(const char* name, void* config);

JNIEXPORT void JNICALL Java_org_kds_Plugin_init_1driver (JNIEnv *env, jobject, jstring str) {
    const char *name = env->GetStringUTFChars(str, NULL);
    std::cout << "Got name from java " << name << std::endl;
    struct {
        JNIEnv* env;
    } config;
    config.env = env;
    init("java", &config);
}
