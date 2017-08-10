#include "GLProcess.h"
#include "GL.h"
GLVAO * glvao;
GLShader * glshader;
GLProgram * glprogram;
	void GLProcess::init(int width , int height){
		glvao = new GLVAO();
		glshader = new GLShader();
		glprogram = new GLProgram();
		
		GLProcess::width = width;
		GLProcess::height = height;
	}
	void GLProcess::step(){
		//¶¥µãÊý×é
		glvao->setVAO(GLProcess::width,GLProcess::height);
		glshader->compileShader();
		glprogram->linkProgram();
	}