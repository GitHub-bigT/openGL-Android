#version 330 core
layout(location = 1) in vec2 vPosition;
layout(location = 2) in vec4 vColor;
out vec4 fColor;
out vec3 fPostion;
void main(){
	gl_Position = vec4(vPosition,0.0f,1.0f);
	fColor = vColor;
	fPostion = vec3(vPosition,0.0f);
}