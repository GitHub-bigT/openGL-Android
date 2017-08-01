#include "interface.h"

GLuint program;
GLfloat coords[] = {
	1.0f,1.0f,
	-1.0f,0.0f,
	0.0f,-0.1f
};
GLfloat colors[] = {
	1.0f,0.0f,0.0f,1.0f,
	0.0f,1.0f,0.0f,1.0f,
	0.0f,0.0f,1.0f,1.0f
};
GLuint locationHandler;
GLuint colorHandler;

const char* vertexShaderSource = "attributee vec2e vPosition;"
								 "void maine() {" 
									"gl_Positione = vec4(vPosition,0.0f,0.0f,0.0f)"
								 "}";
const char* fragmentShaderSource = "";

enum VAO_IDs {Triangles , NumVAOs};
GLuint VAOs[NumVAOs];

JNIEXPORT void JNICALL
Java_ricoh_opengles3_NativeMethod_init(JNIEnv * , jclass , int width, int height){
	LOGI("width: %d , height: %d",width,height);
	program = createProgram();
	//LOGI("program: %d " , program);
	//printGLString("Version", GL_VERSION);
    //printGLString("Vendor", GL_VENDOR);
    //printGLString("Renderer", GL_RENDERER);
    //printGLString("Extensions", GL_EXTENSIONS);
    //printGLString("GL_SHADING_LANGUAGE_VERSION : ", GL_SHADING_LANGUAGE_VERSION);
	
	LOGI("Triangles: %d , NumVAOs: %d",Triangles,NumVAOs);
	LOGI("VAOs: %d ",VAOs[Triangles]);
	glGenVertexArrays(NumVAOs , VAOs);
	LOGI("NumVAOs: %d",NumVAOs);
	LOGI("VAOs: %d ",VAOs[Triangles]);
	glBindVertexArray(VAOs[Triangles]);
	//
	GLboolean b = glIsVertexArray(VAOs[Triangles]);
	LOGI("GLboolean: %d ",b);
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
	glCompileShader(shader);
	
	LOGE("shader ---- : %d", shader);
	
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
	
}

void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);   
}