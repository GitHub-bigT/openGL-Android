#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D depthTexture;

float near = 0.1; 
float far  = 1000.0;
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));    
}

void main()
{ 
	// 深度可视化
	float depth = LinearizeDepth(texture(depthTexture, TexCoords).r) / far;
	color = vec4(vec3(depth), 1.0);
	//color = vec4(vec3(texture(depthTexture, TexCoords).r), 1.0);
	return;
}