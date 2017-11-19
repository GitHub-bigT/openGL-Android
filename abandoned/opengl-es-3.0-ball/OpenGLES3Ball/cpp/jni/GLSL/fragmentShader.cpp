#include "fragmentShader.h"
const char* FragmentShader::readShaderSource(){
	const char* fragmentShader = "#version 300 es\n"
								"out vec4 fColor;\n"
								"void main() {\n"
								"fColor = vec4(1.0f,0.0f,0.0f,1.0f);\n"
								"}\n";
	return fragmentShader;
}