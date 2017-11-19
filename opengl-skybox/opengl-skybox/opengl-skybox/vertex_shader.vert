#version 430 core

layout(location = 1) in vec3 vPosition;

out vec3 textureCoords;

//uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	//gl_Position = projection * view * vec4(vPosition.x , 1.0f - vPosition.y , vPosition.z , 1.0f) ;
	gl_Position = projection * view * vec4(vPosition , 1.0f) ;
	textureCoords = vPosition;
}