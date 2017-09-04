#version 430 core

layout(location = 1) in vec3 vPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	gl_Position =  vec4(vPosition ,1.0f) ;
}