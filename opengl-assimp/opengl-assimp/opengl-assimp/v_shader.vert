#version 430 core
layout (location = 1) in vec3 vPosition;
layout (location = 2) in vec3 vNormal;
layout (location = 3) in vec2 vTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(1.0f - vPosition.x,1.0f - vPosition.y,vPosition.z, 1.0f);
    TexCoords = vTexCoords;
}