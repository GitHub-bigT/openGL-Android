#version 330 core
in vec4 fColor;

in vec2 fTexCoord;
out vec4 oColor;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform float update_alpha;

void main(){
	//oColor = fColor;
	vec4 texColor = mix(texture(ourTexture1,fTexCoord),texture(ourTexture2, fTexCoord), update_alpha);
	//if(texColor.a<0.1) discard;
	//oColor = texColor;
	oColor = texture(ourTexture2, fTexCoord);
	//oColor = vec4(vec3(texture(ourTexture2,fTexCoord)),0.5f);
	//oColor = ;
}
