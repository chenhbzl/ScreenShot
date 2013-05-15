
#include <jni.h>

#include <utils/Log.h>

JavaVM* jvm = 0;

#ifndef CLASS
#error "?"
#endif

#define NAME1(CLZ, FUN) Java_##CLZ##_##FUN
#define NAME2(CLZ, FUN) NAME1(CLZ, FUN)

#define NAME(FUN) NAME2(CLASS, FUN)


JNIEXPORT jint JNICALL NAME(screenshot)(JNIEnv *env, jobject thiz, jstring file) {
    const char *f;
    jint result;
    jboolean copy;
    f = (*env)->GetStringUTFChars(env, file, &copy);
    result = shot_save(f);
    (*env)->ReleaseStringUTFChars(env, file, f);
    return result;
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
    jvm = vm;

    return JNI_VERSION_1_4;
}


