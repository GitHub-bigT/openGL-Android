#version 330 core
layout(location = 1) in vec2 vPosition;
layout(location = 2) in vec4 vColor;
layout(location = 3) in vec2 vTexCoord;
out vec4 fColor;
out vec2 fTexCoord;
void main(){
	gl_Position = vec4(vPosition ,0.0f,1.0f);
	fColor = vColor;
	//fTexCoord = vTexCoord  ;
	fTexCoord = vec2(vTexCoord.x, 1 - vTexCoord.y)  ;
}