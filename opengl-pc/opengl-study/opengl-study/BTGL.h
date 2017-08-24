#ifndef _BTGL_H_
#define _BTGL_H_
#include <GL\glew.h>
#include <iostream>
#include "File.h"
#include <SOIL.h>
#include <glm.hpp>

class BTVaoVbo{
public:
	void initVaoVbo();
	void initSampler(GLuint programId);
	void drawArrays(GLuint programId, float alpha);
private:
	enum VAO_IDs{ TriangleVAO, NumVaoIds };
	enum VBO_IDs{ TriangleVBO, NumVBOIds };
	enum EBO_IDs{ TriangleEBO, NumEBOIds };
	enum TEX_IDs{ Pic1, Pic2, NumTexIds};
	enum Attrib_IDs{ vPosition = 1, vColor = 2, vTexCoord = 3};
	GLuint VAOs[NumVaoIds];
	GLuint VBOs[NumVaoIds];
	GLuint EBOs[NumEBOIds];
	GLuint TEXs[NumTexIds];
};

class BTprogram {
public:
	GLuint loadShader(GLenum type, const char* shaderSource);
	GLuint createProgram();
	void useProgram();
	GLuint programId;
private:
	GLuint v_shader_Id;
	GLuint f_shader_Id;
};

#endif