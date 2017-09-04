#ifndef _BTGL_H_
#define _BTGL_H_
#include <../GL\glew.h>
#include <iostream>
#include "File.h"
//#include <SOIL.h>
#include "../stdImage\stb_image.h"
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include <../GLFW\glfw3.h>
#define M_PI 3.1415926 
//相邻维度之间的变换角度大小。值越小越像趋于球体
#define STEP 12
//顶点个数
#define VERTEX_COUNT (int)(180.0f / STEP * 360.0 / STEP * 2)

class BTVaoVbo{
public:
	//三角形
	void initVaoVbo();
	//球
	void initBallVaoVbo();
	void initSampler(GLuint programId);
	void drawArrays(int type, GLuint programId, float alpha, glm::mat4 viewMatrix, GLfloat aspect);
	
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