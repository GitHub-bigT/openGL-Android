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
	//�����򶥵�����
	void generateBallVertexCoord(int width , int height);
	public:
	//��������
	float vertexData[24][3];
	//������ɫ��attr����
	enum{vPosition=1};
	void setVAO(int width , int height);
};
#ifdef __cplusplus	
}
#endif

#endif