#pragma once

#ifndef _main_h_
#define _main_h_

#include <jni.h>

#include "../Debug/debug.h"
#include "../tools/tools.h"
//#include "../GLSL/vertexShader.h"
//#include "../GLSL/fragmentShader.h"

#include <GLES3/gl3.h>
#include <math.h>


#ifdef __cplusplus
extern "C" {
#endif
JNIEXPORT void JNICALL
Java_ricoh_opengles3ball_NativeMethod_init(JNIEnv* , jclass , int , int);

JNIEXPORT void JNICALL
Java_ricoh_opengles3ball_NativeMethod_step(JNIEnv* , jclass);

GLuint createProgram();	

GLuint loadShder(GLenum type , const char* source);

void  generateBallVertexCoord(int width , int height);
#ifdef __cplusplus	
}
#endif
#endif