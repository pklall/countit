LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

APP_STL:=stlport_static

LOCAL_C_INCLUDES := $(LOCAL_PATH)

LOCAL_MODULE := counter-jni
LOCAL_SRC_FILES := counter-jni.cpp

include $(BUILD_SHARED_LIBRARY)

