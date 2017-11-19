LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_LDLIBS := -llog -lGLESv3

LOCAL_MODULE := main
LOCAL_SRC_FILES += main.cpp
LOCAL_SRC_FILES += ../tools/tools.cpp
LOCAL_SRC_FILES += ../GL/GLProcess.cpp
LOCAL_SRC_FILES += ../GL/GLVAO.cpp
LOCAL_SRC_FILES += ../GL/GLShader.cpp
LOCAL_SRC_FILES += ../GL/GLProgram.cpp
LOCAL_SRC_FILES += ../GLSL/vertexShader.cpp
LOCAL_SRC_FILES += ../GLSL/fragmentShader.cpp

include $(BUILD_SHARED_LIBRARY)