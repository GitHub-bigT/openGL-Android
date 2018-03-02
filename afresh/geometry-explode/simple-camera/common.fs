#version 330 core

in vec2 TexCoords;

uniform sampler2D sam;

void main()
{
	gl_FragColor = texture(sam, TexCoords);
}