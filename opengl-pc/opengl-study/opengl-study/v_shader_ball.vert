#version 330 core

layout(location = 1) in vec2 vPosition;

out vec4 fColor;

uniform mat4 rotate;
uniform mat4 scale;
void main(){
	gl_Position = vec4(vPosition ,0.0f,1.0f) * rotate;
}