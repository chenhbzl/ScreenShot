LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	jni.c \
	screencap.cpp

LOCAL_SHARED_LIBRARIES := \
	libcutils \
	libutils \
	libbinder \
	libskia \
    libui \
    libgui

LOCAL_MODULE := libscreencap_app

LOCAL_MODULE_TAGS := optional
LOCAL_CFLAGS += -D_LARGEFILE_SOURCE -DCLASS=com_chenhbzl_screenshot_jni_NativeScreenShot

LOCAL_C_INCLUDES += \
	external/skia/include/core \
	external/skia/include/effects \
	external/skia/include/images \
	external/skia/src/ports \
	external/skia/include/utils

include $(BUILD_SHARED_LIBRARY)