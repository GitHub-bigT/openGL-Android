#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D sam;

void main()
{    
    FragColor = texture(sam, vec2(TexCoords.x, 1.0f - TexCoords.y));
	//FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
}