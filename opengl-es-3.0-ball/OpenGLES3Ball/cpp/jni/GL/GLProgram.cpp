#include "GL.h"

GLuint GLProgram::linkProgram(GLuint vertexShader , GLuint fragmentShader){
	//LOGE("link program");
	GLuint program = glCreateProgram();
	glAttachShader(program,vertexShader);
	glAttachShader(program,fragmentShader);
	glLinkProgram(program);
	//check program link
	tools->checkProgram(program);
	return program;
}
