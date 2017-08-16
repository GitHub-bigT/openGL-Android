#include "interface.h"

GLuint program;

const char* vertexShaderSource = "#version 300 es\n"
									"layout(location=1) in vec2 vPosition;\n"
									"layout(location=2) in vec4 iColor;\n"
									"out vec4 oColor;\n"
								 "void main() {\n" 
								 " mat4 sr = mat4(1.0f,0.0f,0.0f,0.5f,"
												" 0.0f,1.0f,0.0f,0.5f,"
												" 0.0f,0.0f,1.0f,0.0f,"
												" 0.0f,0.0f,0.0f,1.0f);\n"
									"gl_Position = vec4(vPosition,0.0f,1.0f)*sr;\n"
									"oColor = iColor;\n"
								 "}\n";
const char* fragmentShaderSource = "#version 300 es\n"
									"in vec4 oColor;\n"
									"out vec4 fColor;\n"
									"void main() {\n"
									"fColor = oColor;\n"
									//"fColor = vec4(0.0f,1.0f,0.0f,1.0f);\n"
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
	current = VAOs[Triangles];
	glGenBuffers(NumBuffers,Buffers);
	
	initVAO1(width,height);	
	//initVAO2(width , height);

	GLboolean b = glIsBuffer(Buffers[ArrayBuffer]);
	LOGE("ArrayBuffer bind: %d ",b);
	GLboolean b1 = glIsBuffer(Buffers[OtherBuffer]);
	LOGE("OtherBuffer bind: %d ",b1);
	//create program
	program = createProgram();
	glUseProgram(program);
	
}

void initVAO1(int width, int height){
	LOGI(" init VAO 1");
	float proportion = (float)width / (float)height;
	// init vao
	glBindVertexArray(VAOs[Triangles]);
	Vertex vertices[] = {
		{{0.0f,0.5f*proportion},{0.0f,1.0f,0.0f,1.0f}},
		{{-0.5f,0.0f},{0.0f,0.0f,1.0f,1.0f}},
		{{0.5f,0.0f},{1.0f,0.0f,0.0f,1.0f}},
		{{0.0f,-0.5f*proportion},{1.0f,1.0f,1.0f,1.0f}}
	};
	
	//init buffer
	glBindBuffer(GL_ARRAY_BUFFER,Buffers[ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
	
	glVertexAttribPointer(vPosition,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),0);
	glVertexAttribPointer(iColor,4,GL_FLOAT,GL_FALSE,sizeof(Vertex),(const GLvoid*)8);
	glEnableVertexAttribArray(vPosition);
	glEnableVertexAttribArray(iColor);
	
	
}

void initVAO2(int width , int height){

	glBindVertexArray(VAOs[OtherTriangles]);
	
	//圆心
	GLfloat vCenter[] = {0.0f, 0.0f};
    //半径
	GLfloat vRadius[] = {1.0f, 1.0f * width/height};
	//362个点  1个原点 + 0-360个点
    GLfloat vVertices[724];
    vVertices[0] = 0.0f;
    vVertices[1] = 0.0f;
	
    GLfloat colors[1448];
	colors[0] = 1.0f;
	colors[1] = 0.0f;
	colors[2] = 0.0f;
	colors[3] = 1.0f;

	for (int i = 0; i <= 360; i++) {
		//弧度
        GLfloat radians = M_PI * i / 180.0f;
        vVertices[i*2+2] = vCenter[0] + cos(radians) * vRadius[0];
        vVertices[i*2+3] = vCenter[1] + sin(radians) * vRadius[1];
		//LOGI(" circle coord : (%f,%f) \n",vVertices[i*2],vVertices[i*2+1]);
		//第偶数个顶点颜色为蓝
		if(i%2==0){
			colors[i*4+4] = 0.0f;
			colors[i*4+5] = 0.0f;
			colors[i*4+6] = 1.0f;
			colors[i*4+7] = 1.0f;
		}else{
			colors[i*4+4] = 0.0f;
			colors[i*4+5] = 1.0f;
			colors[i*4+6] = 0.0f;
			colors[i*4+7] = 1.0f;
		}
    }
	
	//init buffer
	glBindBuffer(GL_ARRAY_BUFFER,Buffers[ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vVertices),vVertices,GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition,2,GL_FLOAT,GL_FALSE,0,0);
	
	
	glBindBuffer(GL_ARRAY_BUFFER,Buffers[OtherBuffer]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(colors),colors,GL_STATIC_DRAW);
	glVertexAttribPointer(iColor,4,GL_FLOAT,GL_FALSE,0,0);
	
	glEnableVertexAttribArray(iColor);
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

float gray = 0.0f;
bool add = true;

JNIEXPORT void JNICALL
Java_ricoh_opengles3_NativeMethod_step(JNIEnv * , jclass){
	//if(gray >= 1.0f) {
	//	add = false;
	//	current = VAOs[Triangles];
	//}
	//if(gray<=0.0f) {
	//	add = true;
	//	current = VAOs[OtherTriangles];
	//}
	//if(add) {
	//	gray += 0.01;
	//}else{
	//	gray -= 0.01;
	//}
	//LOGI("gray : %f ",gray);
	glClearColor(gray,gray,gray,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//LOGI("current %d",current);
	glBindVertexArray(current);
	glDrawArrays(GL_TRIANGLE_STRIP,0,4);
	glFlush();
}

JNIEXPORT void JNICALL
Java_ricoh_opengles3_NativeMethod_switchVAO(JNIEnv * , jclass , int index){
	
	//if(index == VAOs[Triangles]) {
	//	current = VAOs[OtherTriangles];
	//}else {
	//	current = VAOs[Triangles];
	//}
}

void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);   
}