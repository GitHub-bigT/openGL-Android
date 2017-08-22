#version 330 core
in vec4 fColor;
in vec3 fPostion;
out vec4 oColor;
void main(){
	oColor = fColor;
	//oColor = vec4(fPostion,1.0f);
}