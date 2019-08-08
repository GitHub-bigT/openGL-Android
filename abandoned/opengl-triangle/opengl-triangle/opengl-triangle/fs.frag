#version 430

out vec4 oColor;
in vec2 textureCoords;

uniform sampler2D sampler1;

void main(){
	oColor = texture(sampler1, textureCoords);
}
