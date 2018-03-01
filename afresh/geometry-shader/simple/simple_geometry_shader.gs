#version 330 core

/*
	* 1.������任Ϊ��ͬ��ͼԪ
	* 2.���ɱ�ԭ������Ķ���
							*/

in vec3 oColor[];
out vec3 gColor;

layout(points) in;
layout(triangle_strip, max_vertices = 5) out;

void buildHouse(vec4 position)
{
	//������һ�������ʱ��ÿ�����㽫��ʹ�������gColor�д����ֵ��������Ƭ����ɫ��������
	gColor = oColor[0];

	gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);    // 1:����
	EmitVertex();

	gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0);    // 2:����
	EmitVertex();

	gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0);    // 3:����
	EmitVertex();

	gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0);    // 4:����
	EmitVertex();

	gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0);    // 5:����
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