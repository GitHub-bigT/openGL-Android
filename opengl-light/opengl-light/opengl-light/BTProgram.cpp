#include "BTGL.h"

void BTProgram::createProgram(){
	TextFile *textfile = new TextFile();
	const char *v_shader_source = textfile->readFile("v_shader_triangle.vert");
	const char *f_shader_source = textfile->readFile("f_shader_triangle.frag");
	//printf("%s\n",f_shader_source);

	this->v_shader_Id = this->loadShader(GL_VERTEX_SHADER, v_shader_source);
	this->f_shader_Id = this->loadShader(GL_FRAGMENT_SHADER, f_shader_source);

	this->programId = glCreateProgram();
	glAttachShader(this->programId, this->v_shader_Id);
	glAttachShader(this->programId, this->f_shader_Id);
	glLinkProgram(this->programId);

	//check
	GLint linkStatus = GL_FALSE;
	glGetProgramiv(this->programId, GL_LINK_STATUS, &linkStatus);
	printf("program link status:%d\n", linkStatus);

	if (linkStatus == GL_INVALID_VALUE)
	{
		printf("compile shader status:%s\n", "GL_INVALID_VALUE");
	}
	if (linkStatus == GL_INVALID_OPERATION)
	{
		printf("compile shader status:%s\n", "GL_INVALID_OPERATION");
	}

	if (!linkStatus)
	{
		GLint infoLen = 0;
		glGetProgramiv(this->programId, GL_INFO_LOG_LENGTH, &infoLen);
		printf("info length:%d\n", infoLen);
		if (infoLen)
		{
			char* buf = (char*)malloc(infoLen);
			if (buf)
			{
				glGetProgramInfoLog(this->programId, infoLen, NULL, buf);
				printf("link program ERROR:\n%s\n", buf);
				free(buf);
			}
			glDeleteProgram(this->programId);
			this->programId = 0;
		}
	}
}
void BTProgram::useProgram(){
	glUseProgram(this->programId);

	//delete shader
	glDeleteShader(this->v_shader_Id);
	glDeleteShader(this->f_shader_Id);
}

GLuint BTProgram::loadShader(GLenum type, const char* shaderSource){
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);

	//check
	GLint compiled = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (type == GL_VERTEX_SHADER)
	{
		printf("vertex shader:\n");
	}
	else
	{
		printf("fragment shader:\n");
	}
	printf("compile shader status:%d\n", compiled);
	if (!compiled)
	{
		GLint infoLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
		printf("info length:%d\n", infoLen);
		if (infoLen)
		{
			char* buf = (char*)malloc(infoLen);
			if (buf)
			{
				glGetShaderInfoLog(shader, infoLen, NULL, buf);
				printf("compile shader ERROR:\n%s\n", buf);
				free(buf);
			}
			glDeleteShader(shader);
			shader = 0;
		}
	}
	return shader;
}