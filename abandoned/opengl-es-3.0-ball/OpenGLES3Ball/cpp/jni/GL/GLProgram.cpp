#include "GL.h"

GLuint GLProgram::linkProgram(GLuint vertexShader , GLuint fragmentShader){
	//LOGE("link program");
	GLProgram::programId = glCreateProgram();
	glAttachShader(GLProgram::programId,vertexShader);
	glAttachShader(GLProgram::programId,fragmentShader);
	glLinkProgram(GLProgram::programId);
	//check program link
	tools->checkProgram(GLProgram::programId);
	return GLProgram::programId;
}

void GLProgram::useProgram(){
	glUseProgram(GLProgram::programId);
}
