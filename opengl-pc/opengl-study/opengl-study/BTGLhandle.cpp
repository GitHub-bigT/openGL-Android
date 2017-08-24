#include "BTGLHandle.h"

void Handle::init(){
	//std::cout << "handle init" << std::endl;
	btVaoVbo = new BTVaoVbo();
	btVaoVbo->initVaoVbo();
	BTprogram* btProgram = new BTprogram();
	programId = btProgram->createProgram();
	btProgram->useProgram();
	btVaoVbo->initSampler(programId);
}
void Handle::drawTriangles(){
	//std::cout << "draw triangle" << std::endl;
	btVaoVbo->drawArrays(programId);
}
Handle::Handle()
{
	printf("here is constructor\n");
}

Handle::~Handle()
{
}