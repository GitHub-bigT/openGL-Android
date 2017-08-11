#include "GL.h"
GLuint GLShader::compileShader(GLenum type , const char* source){

	GLuint shader = glCreateShader(type);
	glShaderSource(shader,1,&source,NULL);
	glCompileShader(shader);
	//check shader
	tools->checkShader(shader,type);
	return shader;
}
