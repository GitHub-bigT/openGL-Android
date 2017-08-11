#include "tools.h"

//��ӡopengl�����Ϣ
void Tools::printGLString(const char *name, GLenum s){
	const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);   
}

//���shader�Ƿ����ɹ�
GLuint Tools::checkShader(GLuint shader , GLenum shaderType){
	//check

	GLint compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		
	LOGE("shader ---- shader compile status: %d",compiled);
		
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
	return shader;
}

//���program�Ƿ�link�ɹ�
GLuint Tools::checkProgram(GLuint program){
	
	 GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
		
		LOGE("program ---- program link status: %d",linkStatus);
		
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
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

//���VAO�Ƿ�bind�ɹ�
GLboolean Tools::checkIsBindVertexArray(GLuint index){
	GLboolean b = glIsVertexArray(index);
	LOGE("vao bind status:%d",b);
	return b;
}
//���VBO�Ƿ�bind�ɹ�
GLboolean Tools::checkIsBindBuffer(GLuint index){
	GLboolean b = glIsBuffer(index);
	LOGE("vbo bind status:%d",b);
	return b;
}