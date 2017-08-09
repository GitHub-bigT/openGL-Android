#ifndef _main_h_
#define _main_h_

#include <jni.h>
#include <math.h>
#include <GLES3/gl3.h>
#include "../Debug/debug.h"
#include "../tools/tools.h"
#include "../GL/GLProcess.h"



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