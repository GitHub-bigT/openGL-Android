#ifndef _GL_h_
#define _GL_h_

#include "../Debug/debug.h"

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
	void setVAO();
};
#ifdef __cplusplus	
}
#endif

#endif