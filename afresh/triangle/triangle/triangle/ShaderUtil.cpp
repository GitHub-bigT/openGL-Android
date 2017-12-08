#include "ShaderUtil.h"

ShaderUtil::ShaderUtil()
{
}

ShaderUtil::~ShaderUtil()
{
}

GLuint ShaderUtil::initShader(const char* shaderSource, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);
	if (isEnableDebug)
	{
		int  success;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			printf("COMPILE SHADER ERRPR %s\n",std::string(infoLog).c_str());
		}
		else
		{
			printf("COMPILE SHADER SUCCESS\n");
		}
	}
	return shader;
}
