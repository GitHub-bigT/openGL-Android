#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform vec2 screenSpaceLightPos;
uniform sampler2D screenTexture;

float near = 0.1; 
float far  = 1000.0;
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));    
}

void main()
{ 
	color = texture(screenTexture, TexCoords) * 10e4;
	return;
}