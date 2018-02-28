#version 330 core

/*
	* 1.将顶点变换为不同的图元
	* 2.生成比原来更多的顶点
							*/
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

void main()
{
	//gl_Position = gl_in[0].gl_Position + vec4(-0.5, 0.0, 0.0, 0.0);
	gl_Position = gl_in[0].gl_Position;
	//gl_PointSize = gl_in[0].gl_PointSize;
	EmitVertex();

	//gl_Position = gl_in[0].gl_Position + vec4(0.0, 0.5, 0.0, 0.0);
	gl_Position = gl_in[1].gl_Position;
	//gl_PointSize = gl_in[1].gl_PointSize;
	EmitVertex();

	//gl_Position = gl_in[0].gl_Position + vec4(0.0, 0.1, 0.0, 0.0);
	gl_Position = gl_in[2].gl_Position;
	//gl_PointSize = gl_in[2].gl_PointSize;
	EmitVertex();

	EndPrimitive();
}