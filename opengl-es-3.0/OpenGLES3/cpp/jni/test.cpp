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
	// VAO是一个对象，其中包含一个或者更多的Vertex Buffer Objects。而VBO是Graphics Card中的一个内存缓冲区，用来保存顶点信息，颜色信息，法线信息，纹理坐标信息和索引信息等等。
	//  VAO在Graphics Card线性的存储几个对象信息，替代了以前发送我们需要的数据到Graphics Card上，这也是Direct3D没有立即模式情况下工作的方法，
	// 这就意味着应用程序不需要传输数据到Graphics Card上而得到较高的性能。
	// 在缓存区中生成顶点数组对象名字，该名字标记可能不是连续的，但是一定是没有使用的，用顶点数组集合的默认状态来初始化。
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
		// 绑定顶点数组对象名字，当前的顶点数组对象只能激活一个.
		// glBindVertexArray绑定VAO，一旦VAO绑定后，使glGenBuffers 创建一个Vertex Buffer Object, 当然仍然需要使用glBindBuffer绑定VBO；
		// 使用glBufferData来初始化和用刚VAO创建的数据分配数据给VBO，再告诉VBO的数据是从VAO而来，需要清理Vertex Attributr Array和Vertex Array Object
		// bind a vertex array object，
		// array is the name of a vertex array object previously returned from a call to glGenVertexArrays, 
		// or zero to break the existing vertex array object binding.
		//  If the bind is successful no change is made to the state of the vertex array object, and any previous vertex array object binding is broken.
		glBindVertexArray(VAO[Cube]);
		
		glGenBuffers(NumVBOs, buffers);
		// When a buffer object is bound to a target, the previous binding for that target is automatically broken.
		glBindBuffer(GL_ARRAY_BUFFER, buffers[Vertices]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts),
			cubeVerts, GL_STATIC_DRAW);
		// 要清楚关联a buffer object和 a vertex attribute并不发生在glBindBuffer(GL_ARRAY_BUFFER),而是发生在glVertexAttribPointer();当你调用glVertexAttribPointer() ，OpenGL 获取缓冲区绑定到GL_ARRAY_BUFFER 并且关联顶点属性，想象一下GL_ARRAY_BUFFER就像一个全局指针。
		glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
		glEnableClientState(GL_VERTEX_ARRAY);
		
		// 前面的GL_ARRAY_BUFFER绑定失效，GL_ARRAY_BUFFER就像一个全局指针
		glBindBuffer(GL_ARRAY_BUFFER, buffers[Colors]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeColors),
			cubeColors, GL_STATIC_DRAW);
		// 取得当前激活的缓存区对象偏移
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
		// 激活顶点数组对象，用于设置顶点数组状态，关联到制定的缓存区对象，然后绘制出来
		glBindVertexArray(VAO[i]);
		// 当有多个glDrawElements时候，就需要用glBindVertexArray绑定VAO对象，告知绘图函数调用那个顶点数组的索引缓存和顶点缓存
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