#pragma once

#define GLEW_STATIC
#include <../GL/glew.h>
#include <../GLFW/glfw3.h>
#include <../glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "ReadFile.h"


class BTBind{
public:
	enum Shape{
		TRIANGLE
	};
	enum VAO_IDs
	{
		TriangleVAO,NumVAOIds
	};
	enum VBO_IDs
	{
		TriangleVBO,NumVBOIds
	};
	GLuint VAOs[NumVAOIds];
	GLuint VBOs[NumVBOIds];
	enum Attrib_IDs
	{
		vPosition = 1
	};

	void init(Shape shape);
	void draw(Shape shape, GLuint programId);

private:
	void initTriangle();
	void drawTriangle(GLuint programId);
};

class BTProgram{
public:
	GLuint programId;
	
	void createProgram();
	void useProgram();
private:
	GLuint v_shader_Id;
	GLuint f_shader_Id;

	GLuint loadShader(GLenum type, const char* shaderSource);
};

class BTError{
public:
	GLenum glCheckError_(const char *file, int line)
	{
		GLenum errorCode;
		while ((errorCode = glGetError()) != GL_NO_ERROR)
		{
			std::string error;
			switch (errorCode)
			{
			case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
			case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
			case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
			case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
			case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
			case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
			}
			std::cout << error << " | " << file << " (" << line << ")" << std::endl;
		}
		return errorCode;
	}
#define glCheckError() glCheckError_(__FILE__, __LINE__) 
};