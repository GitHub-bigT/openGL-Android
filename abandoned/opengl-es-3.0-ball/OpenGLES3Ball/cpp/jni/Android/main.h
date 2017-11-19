#pragma once

#ifndef _main_h_
#define _main_h_

#include <jni.h>

#include "../Debug/debug.h"
#include "../tools/tools.h"
#include "../GL/GLProcess.h"

#include <GLES3/gl3.h>
#include <math.h>


#ifdef __cplusplus
extern "C" {
#endif
JNIEXPORT void JNICALL
Java_ricoh_opengles3ball_NativeMethod_init(JNIEnv* , jclass , int , int);

JNIEXPORT void JNICALL
Java_ricoh_opengles3ball_NativeMethod_step(JNIEnv* , jclass);

#ifdef __cplusplus	
}
#endif
#endif