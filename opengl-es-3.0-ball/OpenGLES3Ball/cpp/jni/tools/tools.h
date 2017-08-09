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

//��ӡopengl�����Ϣ
void printGLString(const char *name, GLenum s);
//���shader�Ƿ����ɹ�
GLuint checkShader(GLuint shader , GLenum shaderType);
//���program�Ƿ�link�ɹ�
GLuint checkProgram(GLuint program);

};
#ifdef __cplusplus	
}
#endif
#endif