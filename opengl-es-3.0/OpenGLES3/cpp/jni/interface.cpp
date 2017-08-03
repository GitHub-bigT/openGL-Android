#include "interface.h"

GLuint program;
//GLfloat coords[] = {
//	1.0f,1.0f,
//	-1.0f,0.0f,
//	0.0f,-0.1f
//};
//GLfloat colors[] = {
//	1.0f,0.0f,0.0f,1.0f,
//	0.0f,1.0f,0.0f,1.0f,
//	0.0f,0.0f,1.0f,1.0f
//};

//GLuint locationHandler;
//GLuint colorHandler;

const char* vertexShaderSource = "#version 300 es\n"
									"layout(location=0) in vec2 vPosition;\n"
								 "void main() {\n" 
									"gl_Position = vec4(vPosition,0.0f,1.0f);\n"
								 "}\n";
const char* fragmentShaderSource = "#version 300 es\n"
									"out vec4 fColor;"
									"void main() {\n"
									"fColor = vec4(0.0f,1.0f,0.0f,1.0f);\n"
									"}\n"
									;

enum VAO_IDs {Triangles , NumVAOs};
enum Buffer_IDs {ArrayBuffer , NumBuffers};
enum Attrib_IDs {vPosition = 0};
//GLuint vPosition;

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLuint NumVertices = 6;

JNIEXPORT void JNICALL
Java_ricoh_opengles3_NativeMethod_init(JNIEnv * , jclass , int width, int height){
	LOGI("width: %d , height: %d",width,height);
	
	//LOGI("program: %d " , program);
	//printGLString("Version", GL_VERSION);
    //printGLString("Vendor", GL_VENDOR);
    //printGLString("Renderer", GL_RENDERER);
    //printGLString("Extensions", GL_EXTENSIONS);
    printGLString("GL_SHADING_LANGUAGE_VERSION : ", GL_SHADING_LANGUAGE_VERSION);
	
	// init vao
	LOGI("Triangles: %d , NumVAOs: %d",Triangles,NumVAOs);
	LOGI("VAOs: %d ",VAOs[Triangles]);
	glGenVertexArrays(NumVAOs , VAOs);
	LOGI("NumVAOs: %d",NumVAOs);
	LOGI("VAOs: %d ",VAOs[Triangles]);
	glBindVertexArray(VAOs[Triangles]);
	//
	GLboolean b = glIsVertexArray(VAOs[Triangles]);
	LOGE("vao bind: %d ",b);
	
	GLfloat vertices[NumVertices][2] = {
		{0.0f,1.0f},
		{-1.0f,0.0f},
		{1.0f,1.0f},
		{1.0f,0.0f},
		{-1.0f,-1.0f},
		{0.0f,-1.0f}
	};
	
	LOGI(" vertices size of :%lu",sizeof(vertices) );
	
	//init buffer
	glGenBuffers(NumBuffers,Buffers);
	glBindBuffer(GL_ARRAY_BUFFER,Buffers[ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
	
	GLboolean b1 = glIsBuffer (Buffers[ArrayBuffer]);
	LOGE("buffer bind: %d ",b1);
	
	//create program
	program = createProgram();
	
	glUseProgram(program);
	
	//vPosition = glGetAttribLocation(program,"vPosition");
	LOGI(" vPosition :%d" , vPosition );
	
	glVertexAttribPointer(vPosition,2,GL_FLOAT,GL_FALSE,0,0);
	glEnableVertexAttribArray(vPosition);
}

GLuint createProgram(){
	program = glCreateProgram();
	GLuint vertexShader = loadShder(GL_VERTEX_SHADER,vertexShaderSource);
	GLuint fragmentShader = loadShder(GL_FRAGMENT_SHADER,fragmentShaderSource);
	
	//LOGI("vertexShader: %d", vertexShader);
	//LOGI("fragmentShader: %d", fragmentShader);
	
	glAttachShader(program,vertexShader);
	glAttachShader(program,fragmentShader);
	glLinkProgram(program);
	
	
	GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
		
		LOGE("program ---- linkStatus: %d", linkStatus);
		
       if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
			
			LOGE("program ---- 得到logo日志的长度: %d", bufLength);
			
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
				
				LOGE("program ---- buf: %s", buf);
				
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    LOGE("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
	return program;
}

GLuint loadShder(GLenum type , const char* source){
	GLuint shader = glCreateShader(type);
	glShaderSource(shader,1,&source,NULL);
	glCompileShader(shader);
	
	//LOGE("shader ---- : %d", shader);
	
	//check
	GLint compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	
	LOGE("shader ---- shader编译status: %d",compiled);
	
    if (!compiled) {
		GLint infoLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
		
		LOGE("shader ---- 得到logo日志的长度: %d", infoLen);
		
		if (infoLen) {
			char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("shader ---- Could not compile shader %d:\n%s\n" , type, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
	return shader;
}

JNIEXPORT void JNICALL
Java_ricoh_opengles3_NativeMethod_step(JNIEnv * , jclass){
	glClearColor(0.0f,0.0f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glBindVertexArray(VAOs[Triangles]);
	glDrawArrays(GL_TRIANGLES,0,NumVertices);
	glFlush();
	
}

void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);   
}