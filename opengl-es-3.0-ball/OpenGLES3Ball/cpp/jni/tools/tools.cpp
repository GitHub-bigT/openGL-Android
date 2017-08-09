#include "tools.h"

void Tools::printGLString(const char *name, GLenum s){
	const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);   
}