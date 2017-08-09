LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_LDLIBS := -llog -lEGL -lGLESv3

LOCAL_MODULE := interface
LOCAL_SRC_FILES := interface.cpp

include $(BUILD_SHARED_LIBRARY)