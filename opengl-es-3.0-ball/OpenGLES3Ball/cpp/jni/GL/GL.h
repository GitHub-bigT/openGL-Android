#ifndef _GL_h_
#define _GL_h_

#include "../Debug/debug.h"
#include "../tools/tools.h"

#include <GLES3/gl3.h>
#include <string>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

extern Tools* tools;

class GLProgram{
	public:
	GLuint linkProgram(GLuint vertexShader , GLuint fragmentShader);
};
class GLShader{
	private:

	public:
	GLuint compileShader(GLenum type , const char* source);
};
class GLVAO{
	private:
	//生成球顶点坐标
	void generateBallVertexCoord(int width , int height);
	public:
	//顶点数据
	float vertexData[24][3];
	//顶点着色器attr索引
	enum{vPosition=1};
	void setVAO(int width , int height);
};
#ifdef __cplusplus	
}
#endif

#endif