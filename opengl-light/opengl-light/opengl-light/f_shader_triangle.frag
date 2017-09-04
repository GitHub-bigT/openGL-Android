#version 430 core

out vec4 oColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main(){
	oColor = vec4(lightColor * objectColor,1.0f);
}
