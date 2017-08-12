#include "GLProcess.h"
#include "GL.h"

GLVAO* glvao;
	void GLProcess::init(int width , int height){
		GLProcess::width = width;
		GLProcess::height = height; 
		//VAO VBO
		glvao = new GLVAO();
		glvao->setVAO(GLProcess::width,GLProcess::height);
		//shader
		VertexShader* vs = new VertexShader();
		const char* vertexShaderSource = vs->readShaderSource();
		FragmentShader* fs = new FragmentShader();
		const char* fragmentShaderSource = fs->readShaderSource();
		GLShader* shader = new GLShader();
		GLuint vShader = shader->compileShader(GL_VERTEX_SHADER,vertexShaderSource);
		GLuint fShader = shader->compileShader(GL_FRAGMENT_SHADER,fragmentShaderSource);
		//program
		GLProgram* program = new GLProgram();
		program->linkProgram(vShader,fShader);
		program->useProgram();
	}
	void GLProcess::step(){
		glvao->drawVAO();
	}