#include "interface.h"

GLuint program;

const char* vertexShaderSource = "#version 300 es\n"
									"layout(location=1) in vec2 vPosition;\n"
									"layout(location=2) in vec4 iColor;\n"
									"out vec4 oColor;\n"
								 "void main() {\n" 
									"gl_Position = vec4(vPosition,0.0f,1.0f);\n"
									"oColor = iColor;\n"
								 "}\n";
const char* fragmentShaderSource = "#version 300 es\n"
									"in vec4 oColor;\n"
									"out vec4 fColor;\n"
									"void main() {\n"
									
									"fColor = oColor;\n"
									"}\n"
									;

enum VAO_IDs {Triangles ,OtherTriangles, NumVAOs};
enum Buffer_IDs {ArrayBuffer ,OtherBuffer, NumBuffers};
enum Attrib_IDs {vPosition = 1 , iColor = 2};
//GLuint vPosition;

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLuint NumVertices = 6;
int current ;

JNIEXPORT void JNICALL
Java_ricoh_opengles3_NativeMethod_init(JNIEnv * , jclass , int width, int height){
	//LOGI("program: %d " , program);
	//printGLString("Version", GL_VERSION);
    //printGLString("Vendor", GL_VENDOR);
    //printGLString("Renderer", GL_RENDERER);
    //printGLString("Extensions", GL_EXTENSIONS);
	printGLString("GL_SHADING_LANGUAGE_VERSION : ", GL_SHADING_LANGUAGE_VERSION);
	
	glGenVertexArrays(NumVAOs , VAOs);
	LOGE("Triangles : %d",VAOs[Triangles]);
	LOGE("OtherTriangles : %d",VAOs[OtherTriangles]);
	current = VAOs[Triangles];
	glGenBuffers(NumBuffers,Buffers);
	
	initVAO1();	
	initVAO2();

	GLboolean b = glIsVertexArray(VAOs[Triangles]);
	LOGE("vao1 Triangles bind: %d ",b);
	GLboolean b1 = glIsVertexArray(VAOs[OtherTriangles]);
	LOGE("vao2 OtherTriangles bind: %d ",b1);
	//create program
	program = createProgram();
	glUseProgram(program);
	
}

void initVAO1(){
	LOGI(" init VAO 1");
	
	// init vao
	glBindVertexArray(VAOs[Triangles]);
	Vertex vertices[] = {
		{{0.0f,1.0f},{1.0f,0.0f,0.0f,1.0f}},
		{{-1.0f,0.0f},{0.0f,1.0f,0.0f,1.0f}},
		{{1.0f,1.0f},{0.0f,1.0f,0.0f,1.0f}},
		{{1.0f,0.0f},{0.0f,0.0f,1.0f,1.0f}},
		{{-1.0f,-1.0f},{0.0f,1.0f,1.0f,1.0f}},
		{{0.0f,-1.0f},{0.0f,1.0f,1.0f,1.0f}}
	};
	
	//init buffer
	glBindBuffer(GL_ARRAY_BUFFER,Buffers[ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
	
	glVertexAttribPointer(vPosition,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),0);
	glVertexAttribPointer(iColor,4,GL_FLOAT,GL_FALSE,sizeof(Vertex),(const GLvoid*)8);
	glEnableVertexAttribArray(vPosition);
	glEnableVertexAttribArray(iColor);
	
	
}

void initVAO2(){

	glBindVertexArray(VAOs[OtherTriangles]);
	
	
	Vertex vertices[] = {
		{{-0.5f,0.5f},{1.0f,0.0f,0.0f,1.0f}},
		{{-0.5f,-0.5f},{1.0f,1.0f,0.0f,1.0f}},
		{{0.5f,0.5f},{1.0f,0.0f,1.0f,1.0f}},
		{{0.5f,0.5f},{1.0f,0.0f,0.0f,1.0f}},
		{{-0.5f,-0.5f},{1.0f,1.0f,0.0f,1.0f}},
		{{0.5f,-0.5f},{1.0f,0.0f,1.0f,1.0f}}
	};
		
	//init buffer
	glBindBuffer(GL_ARRAY_BUFFER,Buffers[OtherBuffer]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

	glVertexAttribPointer(vPosition,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),0);
	glVertexAttribPointer(iColor,4,GL_FLOAT,GL_FALSE,sizeof(Vertex),(const GLvoid*)8);
	glEnableVertexAttribArray(vPosition);
	glEnableVertexAttribArray(iColor);

	
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

float gray = 0.0f;
bool add = true;

JNIEXPORT void JNICALL
Java_ricoh_opengles3_NativeMethod_step(JNIEnv * , jclass){
	if(gray >= 1.0f) {
		add = false;
		current = VAOs[Triangles];
	}
	if(gray<=0.0f) {
		add = true;
		current = VAOs[OtherTriangles];
	}
	if(add) {
		gray += 0.01;
	}else{
		gray -= 0.01;
	}
	LOGI("gray : %f ",gray);
	glClearColor(gray,gray,gray,1.0f);
	
	glClear(GL_COLOR_BUFFER_BIT);
	//LOGI("current %d",current);
	glBindVertexArray(current);
	glDrawArrays(GL_TRIANGLES,0,NumVertices);
	glFlush();
}



JNIEXPORT void JNICALL
Java_ricoh_opengles3_NativeMethod_switchVAO(JNIEnv * , jclass , int index){
	
	if(index == VAOs[Triangles]) {
		current = VAOs[OtherTriangles];
	}else {
		current = VAOs[Triangles];
	}
}

void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);   
}