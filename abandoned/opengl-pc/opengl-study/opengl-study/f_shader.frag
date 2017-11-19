#version 330 core

in vec2 fTexCoord;
out vec4 oColor;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform float update_alpha;

void main(){

	vec4 texColor = mix(texture(ourTexture1,fTexCoord),texture(ourTexture2, fTexCoord), 0.3f);
	//if(texColor.a<0.1) discard;
	oColor = texColor;

}
