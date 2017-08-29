#version 330 core
layout(location = 1) in vec3 vPosition;
layout(location = 3) in vec2 vTexCoord;

out vec2 fTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	gl_Position = projection * view *  model * vec4(vPosition ,1.0f) ;

	//fTexCoord = vTexCoord  ;
	fTexCoord = vec2(vTexCoord.x,  vTexCoord.y) ;
}