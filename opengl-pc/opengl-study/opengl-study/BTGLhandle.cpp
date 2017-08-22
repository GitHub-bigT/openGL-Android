#include "BTGLHandle.h"

void Handle::init(){
	//std::cout << "handle init" << std::endl;
	btVaoVbo = new BTVaoVbo();
	btVaoVbo->initVaoVbo();
	BTprogram* btProgram = new BTprogram();
	btProgram->createProgram();
	btProgram->useProgram();
}
void Handle::drawTriangles(){
	//std::cout << "draw triangle" << std::endl;
	btVaoVbo->drawArrays();
}
Handle::Handle()
{
	printf("here is constructor\n");
}

Handle::~Handle()
{
}