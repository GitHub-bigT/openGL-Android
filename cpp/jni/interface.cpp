#include <interface.h>
#include <GLES2/gl2.h>
#include <string>


JNIEXPORT void JNICALL
Java_ricoh_opengljava_NativeMethod_onSurfaceCreated(JNIEnv *,jclass){
	LOGI("here is interface.cpp 1 ");
}

JNIEXPORT void JNICALL
Java_ricoh_opengljava_NativeMethod_onSurfaceChanged(JNIEnv *,jclass,int width,int height){
	LOGI("here is interface.cpp 2 ");
	init(width,height);
}

JNIEXPORT void JNICALL
Java_ricoh_opengljava_NativeMethod_onDrawFrame(JNIEnv *,jclass){
	LOGI("here is interface.cpp 3 ");
	glClearColor(0.0f,1.0f,0.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void init(int w, int h){
	GLfloat vertices[] = {
		0.0f,1.0f,0.0f,
		-1.0f,-1.0f,0.0f,
		1.0f,0.0f,0.0f,
		};
		GLuint VBO;
		glGenBuffers(1,&VBO);
		LOGI("%d  ,%d",w,h);
		//LOGI("%s",glIsBuffer(VBO));
		glBindBuffer(GL_ARRAY_BUFFER,VBO);
		glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATICE_DRAW);
		//string vertexSource = "dfadfa";
		//LOGI(vertexSource);
		}
