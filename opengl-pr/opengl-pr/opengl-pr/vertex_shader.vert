#version 430 core

layout(location = 1) in vec3 vPosition;
layout(location = 2) in vec2 vTextureCoords;

out vec2 textureCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	gl_Position = vec4(vPosition ,1.0f) ;
	textureCoords = vTextureCoords;
}