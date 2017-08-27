#ifndef _BTGL_H_
#define _BTGL_H_
#include <GL\glew.h>
#include <iostream>
#include "File.h"
//#include <SOIL.h>
#include "stdImage\stb_image.h"
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#define M_PI 3.1415926 

class BTVaoVbo{
public:
	//Èý½ÇÐÎ
	void initVaoVbo();
	//Çò
	void initBallVaoVbo();
	void initSampler(GLuint programId);
	void drawArrays(int type, GLuint programId, float alpha, float rotateAngle);
private:
	enum VAO_IDs{ TriangleVAO,BallVAO, NumVaoIds };
	enum VBO_IDs{ TriangleVBO,BallVBO, NumVBOIds };
	enum EBO_IDs{ TriangleEBO, NumEBOIds };
	enum TEX_IDs{ Pic1, Pic2, NumTexIds};
	enum Attrib_IDs{ vPosition = 1, vColor = 2, vTexCoord = 3};
	GLuint VAOs[NumVaoIds];
	GLuint VBOs[NumVBOIds];
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