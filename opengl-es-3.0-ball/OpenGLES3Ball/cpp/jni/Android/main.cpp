#include "main.h"

Tools *tools;
GLProcess *process;

JNIEXPORT void JNICALL
Java_ricoh_opengles3ball_NativeMethod_init(JNIEnv* , jclass , int width, int height){
	//LOGI("width: %d , height: %d",width,height);
	tools = new Tools();
	tools->printGLString("Version", GL_VERSION);
    //tools->printGLString("Vendor", GL_VENDOR);
    //tools->printGLString("Renderer", GL_RENDERER);
    //tools->printGLString("Extensions", GL_EXTENSIONS);
    //tools->printGLString("Shader language version", GL_SHADING_LANGUAGE_VERSION);
	process->init();
}

JNIEXPORT void JNICALL
Java_ricoh_opengles3ball_NativeMethod_step(JNIEnv* , jclass){
	//LOGI("here is step");
}