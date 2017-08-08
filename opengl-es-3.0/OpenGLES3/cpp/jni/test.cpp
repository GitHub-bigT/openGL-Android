#include <stdlib.h>
#include <stdio.h>
#define GLEW_STATIC
#include <GL/glew.h>
//#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#define M_PI 3.1415926

#pragma comment(lib, "glew32s.lib")

#define BUFFER_OFFSET(offset) ((const GLubyte *) NULL + offset)  
#define NumberOf(array) (sizeof(array)/sizeof(array[0]))  
typedef struct {
	GLfloat x, y, z;
} vec3;
typedef struct {
	vec3 xlate; /* Translation */
	GLfloat angle;
	vec3 axis;
} XForm;
enum { Cube, Cone, NumVAOs };
GLuint VAO[NumVAOs];
GLenum PrimType[NumVAOs];
GLsizei NumElements[NumVAOs];
XForm Xform[NumVAOs] = {
	{ { -2.0, 0.0, 0.0 }, 0.0,{ 0.0, 1.0, 0.0 } },
	{ { 0.0, 0.0, 2.0 }, 0.0,{ 1.0, 0.0, 0.0 } }
};
GLfloat Angle = 0.0;
void
init()
{
	enum { Vertices, Colors, Elements, NumVBOs };
	GLuint buffers[NumVBOs];
	// VAO��һ���������а���һ�����߸����Vertex Buffer Objects����VBO��Graphics Card�е�һ���ڴ滺�������������涥����Ϣ����ɫ��Ϣ��������Ϣ������������Ϣ��������Ϣ�ȵȡ�
	//  VAO��Graphics Card���ԵĴ洢����������Ϣ���������ǰ����������Ҫ�����ݵ�Graphics Card�ϣ���Ҳ��Direct3Dû������ģʽ����¹����ķ�����
	// �����ζ��Ӧ�ó�����Ҫ�������ݵ�Graphics Card�϶��õ��ϸߵ����ܡ�
	// �ڻ����������ɶ�������������֣������ֱ�ǿ��ܲ��������ģ�����һ����û��ʹ�õģ��ö������鼯�ϵ�Ĭ��״̬����ʼ����
	// generate vertex array object names
	glGenVertexArrays(NumVAOs, VAO);

	{
		GLfloat cubeVerts[][3] = {
			{ -1.0, -1.0, -1.0 },
			{ -1.0, -1.0, 1.0 },
			{ -1.0, 1.0, -1.0 },
			{ -1.0, 1.0, 1.0 },
			{ 1.0, -1.0, -1.0 },
			{ 1.0, -1.0, 1.0 },
			{ 1.0, 1.0, -1.0 },
			{ 1.0, 1.0, 1.0 },
		};
		GLfloat cubeColors[][3] = {
			{ 0.0, 0.0, 0.0 },
			{ 0.0, 0.0, 1.0 },
			{ 0.0, 1.0, 0.0 },
			{ 0.0, 1.0, 1.0 },
			{ 1.0, 0.0, 0.0 },
			{ 1.0, 0.0, 1.0 },
			{ 1.0, 1.0, 0.0 },
			{ 1.0, 1.0, 1.0 },
		};
		GLubyte cubeIndices[] = {
			0, 1, 3, 2,
			4, 6, 7, 5,
			2, 3, 7, 6,
			0, 4, 5, 1,
			0, 2, 6, 4,
			1, 5, 7, 3
		};
		// �󶨶�������������֣���ǰ�Ķ����������ֻ�ܼ���һ��.
		// glBindVertexArray��VAO��һ��VAO�󶨺�ʹglGenBuffers ����һ��Vertex Buffer Object, ��Ȼ��Ȼ��Ҫʹ��glBindBuffer��VBO��
		// ʹ��glBufferData����ʼ�����ø�VAO���������ݷ������ݸ�VBO���ٸ���VBO�������Ǵ�VAO��������Ҫ����Vertex Attributr Array��Vertex Array Object
		// bind a vertex array object��
		// array is the name of a vertex array object previously returned from a call to glGenVertexArrays, 
		// or zero to break the existing vertex array object binding.
		//  If the bind is successful no change is made to the state of the vertex array object, and any previous vertex array object binding is broken.
		glBindVertexArray(VAO[Cube]);
		
		glGenBuffers(NumVBOs, buffers);
		// When a buffer object is bound to a target, the previous binding for that target is automatically broken.
		glBindBuffer(GL_ARRAY_BUFFER, buffers[Vertices]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts),
			cubeVerts, GL_STATIC_DRAW);
		// Ҫ�������a buffer object�� a vertex attribute����������glBindBuffer(GL_ARRAY_BUFFER),���Ƿ�����glVertexAttribPointer();�������glVertexAttribPointer() ��OpenGL ��ȡ�������󶨵�GL_ARRAY_BUFFER ���ҹ����������ԣ�����һ��GL_ARRAY_BUFFER����һ��ȫ��ָ�롣
		glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
		glEnableClientState(GL_VERTEX_ARRAY);
		
		// ǰ���GL_ARRAY_BUFFER��ʧЧ��GL_ARRAY_BUFFER����һ��ȫ��ָ��
		glBindBuffer(GL_ARRAY_BUFFER, buffers[Colors]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeColors),
			cubeColors, GL_STATIC_DRAW);
		// ȡ�õ�ǰ����Ļ���������ƫ��
		glColorPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
		glEnableClientState(GL_COLOR_ARRAY);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
			buffers[Elements]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);
		PrimType[Cube] = GL_QUADS;
		NumElements[Cube] = NumberOf(cubeIndices);
	}

	{
		int i, idx;
		float dTheta;
#define NumConePoints 36  
		/* We add one more vertex for the cone's apex */
		GLfloat coneVerts[NumConePoints + 1][3] = {
			{ 0.0, 0.0, 1.0 }
		};
		GLfloat coneColors[NumConePoints + 1][3] = {
			{ 1.0, 1.0, 1.0 }
		};
		GLubyte coneIndices[NumConePoints + 1];
		dTheta = 2 * M_PI / (NumConePoints - 1);
		idx = 1;
		for (i = 0; i < NumConePoints; ++i, ++idx) {
			float theta = i*dTheta;
			coneVerts[idx][0] = cos(theta);
			coneVerts[idx][1] = sin(theta);
			coneVerts[idx][2] = 0.0;
			coneColors[idx][0] = cos(theta);
			coneColors[idx][1] = sin(theta);
			coneColors[idx][2] = 0.0;
			coneIndices[idx] = idx;
		}
		glBindVertexArray(VAO[Cone]);

		glGenBuffers(NumVBOs, buffers);
		glBindBuffer(GL_ARRAY_BUFFER, buffers[Vertices]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(coneVerts),
			coneVerts, GL_STATIC_DRAW);
		glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
		glEnableClientState(GL_VERTEX_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, buffers[Colors]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(coneColors),
			coneColors, GL_STATIC_DRAW);
		glColorPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
		glEnableClientState(GL_COLOR_ARRAY);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
			buffers[Elements]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			sizeof(coneIndices), coneIndices, GL_STATIC_DRAW);
		PrimType[Cone] = GL_TRIANGLE_FAN;
		NumElements[Cone] = NumberOf(coneIndices);
	}
	//glEnable(GL_DEPTH_TEST);
}


void mouse(int button, int state, int x, int y) {
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			glutIdleFunc(NULL);
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			glutIdleFunc(NULL);
		}
		break;
	default:
		break;
	}
}

void display()
{
	int i;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glRotatef(Angle, 0.0, 1.0, 0.0);
	for (i = 0; i < NumVAOs; ++i) {
		glPushMatrix();
		glTranslatef(Xform[i].xlate.x, Xform[i].xlate.y,
			Xform[i].xlate.z);
		glRotatef(Xform[i].angle, Xform[i].axis.x,
			Xform[i].axis.y, Xform[i].axis.z);
		// �������������������ö�������״̬���������ƶ��Ļ���������Ȼ����Ƴ���
		glBindVertexArray(VAO[i]);
		// ���ж��glDrawElementsʱ�򣬾���Ҫ��glBindVertexArray��VAO���󣬸�֪��ͼ���������Ǹ������������������Ͷ��㻺��
		glDrawElements(PrimType[i], NumElements[i],
			GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));
		glPopMatrix();
	}
	glPopMatrix();
	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(250, 250);

	glutInitWindowPosition(100, 100);
	glutInitContextVersion(3, 1);

	glutCreateWindow("hello");
	glewInit();
	init();
	glutDisplayFunc(display);
	//glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	//glutMotionFunc(move);
	//glutKeyboardFunc(keyboardfunc);
	glutMainLoop();

	return 0;
}