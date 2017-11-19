#include "vertexShader.h"

const char* VertexShader::readShaderSource(){
	const char* vertexShaderSource = "#version 300 es\n"
									"layout(location=1) in vec3 vPosition;\n"
									"void main() {\n" 
									"gl_Position = vec4(vPosition,1.0f);\n"
									"}\n";
	return vertexShaderSource;
}