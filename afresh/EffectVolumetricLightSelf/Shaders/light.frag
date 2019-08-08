#version 330 core
out vec4 color;

in vec3 vColor;
in float vWeight;
uniform vec3 lightColor;

void main()
{ 
	color = vec4(lightColor,vWeight);
	return;
}