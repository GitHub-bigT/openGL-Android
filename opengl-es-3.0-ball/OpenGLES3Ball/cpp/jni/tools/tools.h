#ifndef _tools_h_
#define _tools_h_

#include <GLES3/gl3.h>
#include "../Debug/debug.h"

#ifdef __cplusplus
extern "C" {
#endif
class Tools{
	
public :

void printGLString(const char *name, GLenum s);

};
#ifdef __cplusplus	
}
#endif
#endif