#version 330 core

/*
	* 1.将顶点变换为不同的图元
	* 2.生成比原来更多的顶点
							*/

in vec3 oColor[];
out vec3 gColor;

layout(points) in;
layout(triangle_strip, max_vertices = 5) out;

void buildHouse(vec4 position)
{
	//当发射一个顶点的时候，每个顶点将会使用最后在gColor中储存的值，来用于片段着色器的运行
	gColor = oColor[0];

	gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);    // 1:左下
	EmitVertex();

	gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0);    // 2:右下
	EmitVertex();

	gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0);    // 3:左上
	EmitVertex();

	gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0);    // 4:右上
	EmitVertex();

	gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0);    // 5:顶部
	gColor = vec3(1.0, 1.0, 1.0);
	EmitVertex();

	EndPrimitive();
}

void main()
{
	//gl_Position = gl_in[0].gl_Position + vec4(-0.5, 0.0, 0.0, 0.0);
	//gl_Position = gl_in[0].gl_Position;
	//gl_PointSize = gl_in[0].gl_PointSize;
	//EmitVertex();

	//gl_Position = gl_in[0].gl_Position + vec4(0.0, 0.5, 0.0, 0.0);
	//gl_Position = gl_in[1].gl_Position;
	//gl_PointSize = gl_in[1].gl_PointSize;
	//EmitVertex();

	//gl_Position = gl_in[0].gl_Position + vec4(0.0, 0.1, 0.0, 0.0);
	//gl_Position = gl_in[2].gl_Position;
	//gl_PointSize = gl_in[2].gl_PointSize;
	//EmitVertex();

	//EndPrimitive();

	buildHouse(gl_in[0].gl_Position);
}