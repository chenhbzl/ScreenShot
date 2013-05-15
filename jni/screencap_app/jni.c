
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
	__android_log_print(ANDROID_LOG_INFO, "chenhb",
			"screenshot start \n");
    f = (*env)->GetStringUTFChars(env, file, &copy);
	__android_log_print(ANDROID_LOG_INFO, "chenhb",
			"screenshot 1 \n");
    result = shot_save(f);
	__android_log_print(ANDROID_LOG_INFO, "chenhb",
			"screenshot 2 \n");
    (*env)->ReleaseStringUTFChars(env, file, f);
	__android_log_print(ANDROID_LOG_INFO, "chenhb",
			"screenshot end \n");
    return result;
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
    jvm = vm;

    return JNI_VERSION_1_4;
}


