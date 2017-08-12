#include "main.h"

Tools *tools;
GLProcess *process;

JNIEXPORT void JNICALL
Java_ricoh_opengles3ball_NativeMethod_init(JNIEnv* , jclass , int width, int height){
	tools = new Tools();
	tools->printGLString("Version", GL_VERSION); 
	process = new GLProcess();
	process->init(width,height);
}


JNIEXPORT void JNICALL
Java_ricoh_opengles3ball_NativeMethod_step(JNIEnv* , jclass){
	process->step();	
}