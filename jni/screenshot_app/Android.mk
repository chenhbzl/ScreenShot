LOCAL_PATH:= $(call my-dir)

##first lib
include $(CLEAR_VARS)

LOCAL_MODULE := libscreenshot_app

LOCAL_MODULE_TAGS := optional

LOCAL_SHARED_LIBRARIES := libcutils libz
LOCAL_STATIC_LIBRARIES := libpng
LOCAL_C_INCLUDES += external/zlib

LOCAL_CFLAGS += -D_LARGEFILE_SOURCE -DCLASS=com_chenhbzl_screenshot_jni_NativeScreenShot

LOCAL_SRC_FILES := \
    screenshot.c \
    jni.c

include $(BUILD_SHARED_LIBRARY)