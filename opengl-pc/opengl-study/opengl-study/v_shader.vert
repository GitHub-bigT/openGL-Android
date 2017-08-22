#version 330 core
layout(location = 1) in vec2 vPostion;
layout(location = 2) in vec4 vColor;
out vec4 fColor;
void main(){
	gl_Position = vec4(vPostion,0.0f,1.0f);
	fColor = vColor;
}