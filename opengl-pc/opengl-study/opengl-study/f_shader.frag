#version 330 core
in vec4 fColor;
in vec3 fPostion;

in vec2 fTexCoord;
out vec4 oColor;

uniform sampler2D ourTexture;

void main(){
	//oColor = fColor;
	//oColor = vec4(fPostion,1.0f);
	oColor = texture(ourTexture,fTexCoord);
}