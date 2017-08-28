#version 330 core

layout(location = 1) in vec3 vPosition;

out vec4 fColor;

uniform mat4 rotate;
uniform mat4 scale;



void main(){
	gl_Position = rotate * vec4(vPosition ,1.0f) ;
}