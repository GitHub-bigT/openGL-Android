#include <interface.h>

const char*  vertexSource = "attribute vec3 vPosition;\n"
							"attribute vec4 vColor;"
							"varying vec4 v_fragmentColor;"
										"void main() {\n"
										"gl_Position = vec4(vPosition,1.0);\n"
										"v_fragmentColor = vColor;"
										"}\n";
										
const char*  fragmentSource = "varying vec4 v_fragmentColor;"
								"void main()"
								"{"
								"gl_FragColor = v_fragmentColor;"
								"}";
										
//const char*  vertexSource =
//    "attribute vec4 vPosition;\n"
//    "void main() {\n"
//    "  gl_Position = vPosition;\n"
//    "}\n";
		
//const char* fragmentSource = "precision mediump float;\n"
//								"uniform vec4 vColor;\n"
//								"void main(){\n"
//									"gl_FragColor = vColor;\n"	
//								"}\n";

//auto gFragmentShader =
//    "precision mediump float;\n"
//    "void main() {\n"
//    "  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
//    "}\n";
								
GLuint gProgram;
GLuint gPostionHandle;
GLuint gColorHandle;

GLfloat vertices[] = {
		0.0f,1.0f,0.0f,
		-1.0f,-1.0f,0.0f,
		1.0f,0.0f,0.0f,
		};
//GLfloat color[] = {
//	0.0f,0.0f,1.0f,1.0f
//};
GLfloat color[] = { 0, 1,0, 1,  1,0,0, 1, 0, 0, 1, 1};
		
JNIEXPORT void JNICALL
Java_ricoh_opengljava_NativeMethod_onSurfaceCreated(JNIEnv *,jclass){
	LOGI("here is interface.cpp 1 ");
}

JNIEXPORT void JNICALL
Java_ricoh_opengljava_NativeMethod_onSurfaceChanged(JNIEnv *,jclass,int width,int height){
	LOGI("here is interface.cpp 2 ");
	init(width,height);
}

float grey = 0.0f;
bool isPlus = true;

JNIEXPORT void JNICALL
Java_ricoh_opengljava_NativeMethod_onDrawFrame(JNIEnv *,jclass){
	if(grey>=1.00f){
		isPlus = false;
	}
	if(grey<=0.0f){
		isPlus = true;
	}
	if(isPlus){
		grey+=0.01f;
	}else{
		grey -= 0.01f;
	}
	//LOGI("grey   %f",grey);
	glClearColor(grey,grey,grey,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(gProgram);
	glVertexAttribPointer(gPostionHandle, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(gColorHandle, 4, GL_FLOAT, GL_FALSE, 0, color);
	//glUniform4fv(gColorHandle,1,color);
	glEnableVertexAttribArray(gPostionHandle);
	glEnableVertexAttribArray(gColorHandle);
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
		gPostionHandle = glGetAttribLocation(gProgram,"vPosition");
		gColorHandle = glGetAttribLocation(gProgram,"vColor");
		//gColorHandle = glGetUniformLocation(gProgram,"vColor");	
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
		
		//GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("Could not compile shader %d:\n%s\n",
                            shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
	}
	return shader;
}
