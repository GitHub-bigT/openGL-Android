#pragma once

#define GLEW_STATIC
#include <../GL/glew.h>
#include <../GLFW/glfw3.h>
#include <../glm/gtc/matrix_transform.hpp>
#include <../glm/gtc/type_ptr.hpp>
#include <iostream>
#include "ReadFile.h"
#include "BTShader.h"
#include "BTCamera.h"

#include <../stdImage/stb_image.h>


class BTBind{
public:
	enum Shape{
		TRIANGLE
	};
	enum VAO_IDs
	{
		TriangleVAO,LightVAO,NumVAOIds
	};
	enum VBO_IDs
	{
		TriangleVBO, NumVBOIds
	};
	enum TEX_IDs
	{
		Container,Container_Specular,Emission,NumTEXIds
	};
	GLuint VAOs[NumVAOIds];
	GLuint VBOs[NumVBOIds];
	GLuint TEXs[NumTEXIds];

	enum Attrib_IDs
	{
		vPosition = 1,
		vNormal = 2,
		vTexCoords = 3
	};

	void init(Shape shape);
	void draw(Shape shape, BTShader *bt_shader, BTShader *bt_shader_lamp, glm::mat4 viewMatrix, GLfloat Zoom, glm::vec3 cameraPostion);

private:
	void initTriangle();
	void drawTriangle(BTShader *bt_shader, BTShader *bt_shader_lamp, glm::mat4 viewMatrix, GLfloat Zoom, glm::vec3 cameraPostion);
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