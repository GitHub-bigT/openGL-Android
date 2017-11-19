
#ifndef _GLProcess_h_
#define _GLProcess_h_

#include <GLES3/gl3.h>
#include "../Debug/debug.h"
#include "../GLSL/fragmentShader.h"
#include "../GLSL/vertexShader.h"


#ifdef __cplusplus
extern "C" {
#endif
class GLProcess {
	public:
	//手机的宽高
	int width ;
	int height;
	void init(int width , int height);
	void step();
};
#ifdef __cplusplus	
}
#endif
#endif