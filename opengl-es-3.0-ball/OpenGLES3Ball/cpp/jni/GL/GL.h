#ifndef _GL_h_
#define _GL_h_

#include "../Debug/debug.h"

#include <GLES3/gl3.h>

#ifdef __cplusplus
extern "C" {
#endif
class GLProgram{
	public:
	void linkProgram();
};
class GLShader{
	public:
	void compileShader();
};
class GLVAO{
	public:
	void setVAO(int width , int height);
};
#ifdef __cplusplus	
}
#endif

#endif