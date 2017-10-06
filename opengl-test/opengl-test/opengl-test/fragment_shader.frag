#version 430 core

out vec4 oColor;
in vec2 TexCoords;

uniform sampler2D sampler1;

void main(){
	oColor = vec4(1.0f,0.0f,0.0f,1.0f) + texture(sampler1,TexCoords);
	//oColor = texture(sampler1,textureCoords);
}
