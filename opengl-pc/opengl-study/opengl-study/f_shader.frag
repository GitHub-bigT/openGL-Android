#version 330 core
in vec4 fColor;
in vec3 fPostion;

in vec2 fTexCoord;
out vec4 oColor;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main(){
	//oColor = fColor;
	//oColor = vec4(fPostion,1.0f);
	//oColor = texture(ourTexture1,fTexCoord);
	oColor = mix(texture(ourTexture1,fTexCoord),texture(ourTexture2,fTexCoord),0.5);
}