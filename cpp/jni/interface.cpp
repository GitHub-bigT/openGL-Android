#include <interface.h>

const char*  vertexSource = "attribute vec4 vPostion;\n"
										"void main() {\n"
										"gl_Position = vPostion;\n"
										"}\n";
//const char*  vertexSource =
//    "attribute vec4 vPosition;\n"
//    "void main() {\n"
//    "  gl_Position = vPosition;\n"
//    "}\n";
		
const char* fragmentSource = 
								"void main(){\n"
									"gl_FragColor = vec4(1.0f,0.0f,0.0f,1.0f);"	
								"}\n";
//auto gFragmentShader =
//    "precision mediump float;\n"
//    "void main() {\n"
//    "  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
//    "}\n";
								
GLuint gProgram;
GLuint gPostionHandle;
GLfloat vertices[] = {
		0.0f,1.0f,0.0f,
		-1.0f,-1.0f,0.0f,
		1.0f,0.0f,0.0f,
		};
		
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
	//LOGI("here is interface.cpp 3 ");
	glClearColor(0.0f,1.0f,0.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(gProgram);
	glVertexAttribPointer(gPostionHandle, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glEnableVertexAttribArray(gPostionHandle);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}


		
void init(int w, int h){
		//GLuint VBO;
		//glGenBuffers(1,&VBO);
		LOGI("%d  ,%d",w,h);
		//LOGI("%s",glIsBuffer(VBO));
		//glBindBuffer(GL_ARRAY_BUFFER,VBO);
		//glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
		
		gProgram = createProgram(vertexSource,fragmentSource);
		gPostionHandle = glGetAttribLocation(gProgram,"vPostion");
}

GLuint createProgram(const char*pVertexSource , const char* pFragmentSource){
	GLuint vertexShader = loadShader(GL_VERTEX_SHADER,pVertexSource);
	GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER,pFragmentSource);

	GLuint program = glCreateProgram();
	if(program){
		glAttachShader(program,vertexShader);
		glAttachShader(program,fragmentShader);
		glLinkProgram(program);
	}
	return program;
}

GLuint loadShader(GLenum shaderType , const char* pSource){
	GLuint shader = glCreateShader(shaderType);
	if(shader){
		glShaderSource(shader,1,&pSource,NULL);
		glCompileShader(shader);
		//check shader
		GLint compiled = GL_FALSE;
		LOGI("shader compiled status pre   %d",compiled);
		glGetShaderiv(shader,GL_COMPILE_STATUS,&compiled);
		LOGI("shader compiled status    %d",compiled);
	}
	return shader;
}