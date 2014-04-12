LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := counter-jni
LOCAL_SRC_FILES := counter-jni.c

include $(BUILD_SHARED_LIBRARY)
