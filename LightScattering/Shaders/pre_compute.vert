#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(position, 1.0f);
	TexCoords = texCoords;
    //vec4 MinMaxUV = vec4(-1, -1, 1, 1);
    //vec2 uv[4];
    //uv[0] = MinMaxUV.xy;
    //uv[1] = MinMaxUV.xw;
    //uv[2] = MinMaxUV.zy;
    //uv[3] = MinMaxUV.zw;
    //TexCoords = uv[gl_VertexID];
} 