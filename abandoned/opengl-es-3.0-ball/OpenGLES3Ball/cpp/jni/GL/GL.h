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
	
	GLuint programId;
	
	GLuint linkProgram(GLuint vertexShader , GLuint fragmentShader);
	void useProgram();
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
	
	void setVAO(int width , int height);
	
	void drawVAO();
};
#ifdef __cplusplus	
}
#endif

#endif