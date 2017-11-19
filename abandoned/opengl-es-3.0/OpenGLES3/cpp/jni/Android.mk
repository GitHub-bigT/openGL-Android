LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_LDLIBS := -llog -lEGL -lGLESv3

LOCAL_MODULE := interface
#可选变量，表示头文件的搜索路径。
#默认的头文件的搜索路径是LOCAL_PATH目录
LOCAL_C_INCLUDES += $(LOCAL_PATH)/glm/
LOCAL_SRC_FILES := interface.cpp


include $(BUILD_SHARED_LIBRARY)