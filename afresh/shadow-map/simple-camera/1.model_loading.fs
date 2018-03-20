#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D sam;

void main()
{    
    FragColor = texture(sam, TexCoords);
	//FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
}