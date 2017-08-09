LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_LDLIBS := -llog -lGLESv3

LOCAL_MODULE := main
LOCAL_SRC_FILES += main.cpp
LOCAL_SRC_FILES += ../tools/tools.cpp

include $(BUILD_SHARED_LIBRARY)