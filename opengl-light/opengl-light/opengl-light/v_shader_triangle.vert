#version 430 core

layout(location = 1) in vec3 vPosition;
layout(location = 2) in vec3 vNormal;

out vec3 Normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	gl_Position =projection * view * model * vec4(vPosition.x , vPosition.y , vPosition.z ,1.0f) ;
	//Normal = vNormal;
	//正规矩阵
	Normal = mat3(transpose(inverse(model))) * vNormal;
	//Normal = mat3(model) * vNormal;
	//世界坐标
	fragPos = vec3(model * vec4(vPosition,1.0f));
}