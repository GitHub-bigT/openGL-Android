#version 330 core
out vec4 FragColor;

in vec2 gTexCoords;

uniform sampler2D sam;

void main()
{    
    FragColor = texture(sam, gTexCoords);
}