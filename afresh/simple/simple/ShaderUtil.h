#pragma once
#include <glad/glad.h>
#include <iostream>
#include <string>

class ShaderUtil
{
public:
	GLboolean isEnableDebug = GL_FALSE;
private:

public:
	ShaderUtil();
	~ShaderUtil();
	GLuint initShader(const char* shaderSource, GLenum shaderType);
private:

};

