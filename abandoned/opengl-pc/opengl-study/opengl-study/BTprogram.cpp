#include "BTGL.h"

GLuint BTprogram::createProgram(){
	//std::cout << "create program" << std::endl;
	TextFile* textFile = new TextFile();
	char* v_shader = textFile->readFile("v_shader.vert");
	v_shader_Id = loadShader(GL_VERTEX_SHADER, v_shader);

	char* f_shader = textFile->readFile("f_shader.frag");
	f_shader_Id = loadShader(GL_FRAGMENT_SHADER, f_shader);

	programId = glCreateProgram();
	glAttachShader(programId, v_shader_Id);
	glAttachShader(programId, f_shader_Id);
	glLinkProgram(programId);

	//check
	GLint linkStatus = GL_FALSE;
	glGetProgramiv(programId,GL_LINK_STATUS,&linkStatus);
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
		glGetProgramiv(programId,GL_INFO_LOG_LENGTH,&infoLen);
		printf("info length:%d\n", infoLen);
		if (infoLen)
		{
			char* buf = (char*)malloc(infoLen);
			if (buf)
			{
				glGetProgramInfoLog(programId, infoLen, NULL, buf);
				printf("link program ERROR:\n%s\n", buf);
				free(buf);
			}
			glDeleteProgram(programId);
			programId = 0;
		}
	}
	return programId;
}

void BTprogram::useProgram(){
	glUseProgram(programId);
	
	//delete shader
	glDeleteShader(v_shader_Id);
	glDeleteShader(f_shader_Id);
}

GLuint BTprogram::loadShader(GLenum type , const char* shaderSource){
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);

	//check
	GLint compiled = GL_FALSE;
	glGetShaderiv(shader,GL_COMPILE_STATUS,&compiled);
	if (type==GL_VERTEX_SHADER)
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
		glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&infoLen);
		printf("info length:%d\n",infoLen);
		if (infoLen)
		{
			char* buf = (char*)malloc(infoLen);
			if (buf)
			{
				glGetShaderInfoLog(shader,infoLen,NULL,buf);
				printf("compile shader ERROR:\n%s\n", buf);
				free(buf);
			}
			glDeleteShader(shader);
			shader = 0;
		}
	}
	return shader;
}