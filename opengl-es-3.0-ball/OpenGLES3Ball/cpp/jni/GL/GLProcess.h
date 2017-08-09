#ifndef _GLProcess_h_
#define _GLProcess_h_

#include <GLES3/gl3.h>
#include "../Debug/debug.h"

#ifdef __cplusplus
extern "C" {
#endif
class GLProcess {
	public:
	void init();
	void step();
};
#ifdef __cplusplus	
}
#endif
#endif