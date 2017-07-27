#include <jni.h>
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
#ifdef __cplusplus
}
#endif
