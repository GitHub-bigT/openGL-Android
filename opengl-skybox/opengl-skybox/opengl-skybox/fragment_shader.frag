#version 430 core

out vec4 oColor;
in vec3 textureCoords;

uniform samplerCube sample_cube;

void main(){
	//oColor = vec4(1.0f,0.0f,0.0f,0.0f);
	oColor = texture(sample_cube,textureCoords);
}
