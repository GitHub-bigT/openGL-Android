#ifndef _tools_h_
#define _tools_h_

#include <GLES3/gl3.h>
#include <malloc.h>
#include "../Debug/debug.h"

#ifdef __cplusplus
extern "C" {
#endif
class Tools{
	
public :

//打印opengl相关信息
void printGLString(const char *name, GLenum s);
//检查shader是否编译成功
GLuint checkShader(GLuint shader , GLenum shaderType);
//检查program是否link成功
GLuint checkProgram(GLuint program);

};
#ifdef __cplusplus	
}
#endif
#endif