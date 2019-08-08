#version 430

layout(location = 1) in vec2 vPosition;
//layout(location = 2) in vec2 vTextureCoords;

//out vec2 textureCoords;

//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;

void main(){
	//gl_Position = projection * view * vec4(vPosition.x , 1.0f - vPosition.y , vPosition.z , 1.0f) ;
	gl_Position = vec4(vPosition, 0.0f, 1.0f);
	//textureCoords = vTextureCoords;
}