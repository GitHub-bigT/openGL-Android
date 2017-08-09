#include <jni.h>
#include <GLES3/gl3.h>
#include <string>
#include "debug.h"
#include <math.h> 

#ifdef __cplusplus
extern "C" {
#endif	

struct Vertex {
	float pos[2];
	float color[4];
};
	
JNIEXPORT void JNICALL

Java_ricoh_opengles3_NativeMethod_init(JNIEnv * , jclass , int , int);

JNIEXPORT void JNICALL
Java_ricoh_opengles3_NativeMethod_step(JNIEnv * , jclass);

JNIEXPORT void JNICALL
Java_ricoh_opengles3_NativeMethod_switchVAO(JNIEnv * , jclass , int);

GLuint createProgram();	

GLuint loadShder(GLenum type , const char* source);

void printGLString(const char *name, GLenum s);

void initVAO1();

void initVAO2(int ,int );

#ifdef __cplusplus
}
#endif