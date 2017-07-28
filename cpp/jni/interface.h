#include <jni.h>
#include <GLES2/gl2.h>
#include <string>
#include "Debug.hpp"

#ifdef  __cplusplus
extern "C" {
#endif
JNIEXPORT void JNICALL
Java_ricoh_opengljava_NativeMethod_onSurfaceCreated(JNIEnv *,jclass);
JNIEXPORT void JNICALL
Java_ricoh_opengljava_NativeMethod_onSurfaceChanged(JNIEnv *,jclass,int,int);
JNIEXPORT void JNICALL
Java_ricoh_opengljava_NativeMethod_onDrawFrame(JNIEnv *,jclass);
void init(int w, int h);
GLuint createProgram(const char* vertexShader , const char* fragmentShader);
GLuint loadShader(GLenum shaderType , const char* pSource);
#ifdef __cplusplus
}
#endif
