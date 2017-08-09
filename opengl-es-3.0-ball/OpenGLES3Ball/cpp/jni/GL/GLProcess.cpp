#include "GLProcess.h"
#include "GL.h"
GLVAO * glvao;
GLShader * glshader;
GLProgram * glprogram;
	void GLProcess::init(){
		glvao = new GLVAO();
		glshader = new GLShader();
		glprogram = new GLProgram();
		
		glvao->setVAO();
		glshader->compileShader();
		glprogram->linkProgram();
	}
	void GLProcess::step(){
		
	}