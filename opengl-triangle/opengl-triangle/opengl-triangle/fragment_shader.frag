#version 430 core

out vec4 oColor;
in vec2 textureCoords;

uniform sampler2D sampler1;

void main(){
	//oColor = mix(vec4(texture(sampler1,textureCoords),1.0f),vec4(1.0f,0.0f,0.0f,0.0f),0.0);
	//oColor = mix(texture(sampler1, textureCoords), vec4(1.0f,0.0f,0.0f,1.0f), 0.0);
	oColor = texture(sampler1,textureCoords);
}
