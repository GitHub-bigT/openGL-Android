#include <jni.h>
#include <GLES3/gl3.h>
#include <string>
#include "debug.h"

#ifdef __cplusplus
extern "C" {
#endif	
	
JNIEXPORT void JNICALL

Java_ricoh_opengles3_NativeMethod_init(JNIEnv * , jclass , int , int);

JNIEXPORT void JNICALL
Java_ricoh_opengles3_NativeMethod_step(JNIEnv * , jclass);

GLuint createProgram();	

GLuint loadShder(GLenum type , const char* source);

void printGLString(const char *name, GLenum s);

#ifdef __cplusplus
}
#endif