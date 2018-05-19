#include "AnimationPlayer.h"

static    PIXELFORMATDESCRIPTOR pfd =
{
	sizeof(PIXELFORMATDESCRIPTOR),    //������ʽ�������Ĵ�С
	1,                                //�汾��
	PFD_DRAW_TO_WINDOW |              //��ʽ֧�ִ���
	PFD_SUPPORT_OPENGL |              //��ʽ����֧��OpenGL
	PFD_DOUBLEBUFFER,                 //����֧��˫����
	PFD_TYPE_RGBA,                    //����RGBA ��ʽ
	24,                               //ѡ��ɫ�����
	0, 0, 0, 0, 0, 0, 0, 0,           //����RGBA
	0,                                //���ۼӻ���
	0, 0, 0, 0,                       //���Ծۼ�λ
	32,                               //32λZ-����(��Ȼ���)
	0,                                //���ɰ建��
	0,                                //�޸�������
	PFD_MAIN_PLANE,                   //����ͼ��
	0,                                //Reserved
	0, 0, 0                           //���Բ�����
};

static GLfloat vertices[] =
{
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
	 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
	 1.0f, -1.0f, 0.0f, 1.0f, 0.0f
};

static GLuint indices[] =
{
	0, 1, 2, 0, 2, 3
};

const char* vertexShader = "#version 430 core\n\
layout(location = 0) in vec3 iPos;\n\
layout(location = 1) in vec2 iTexCoord;\n\
out vec2 oTexCoord;\n\
void main()\n\
{\n\
gl_Position = vec4(iPos.x, iPos.y, iPos.z, 1.0f);\n\
oTexCoord = vec2(iTexCoord.x, 1 - iTexCoord.y);\n\
}\n";

const char* fragmentShader = "#version 430 core\n\
in vec2 oTexCoord;\n\
uniform sampler2D s_texture;\n\
out vec4 color;\n\
void main()\n\
{\n\
	color = texture(s_texture, oTexCoord);\n\
	//color = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n\
}\n";

AnimationPlayer::AnimationPlayer(QWidget *w, HWND h, int fr, QVector<QImage> &v)
{
	this->isRun = true;
	this->mWinDC = NULL;
	this->mWinRC = NULL;
	this->mVaoId = 0;
	this->mVboId = 0;
	this->mEboId = 0;
	this->mProgramId = 0;
	this->mHWND = h;
	this->mWidget = w;
	this->mImageArray = v;
	this->mWidth = w->width();
	this->mHeight = w->height();
	this->mFrameRate = fr;
	this->isOpenDebugInfo = false;
	this->isOpenShaderDebugInfo = false;
}

AnimationPlayer::AnimationPlayer()
{
	this->isRun = true;
	this->mWinDC = NULL;
	this->mWinRC = NULL;
	this->mVaoId = 0;
	this->mVboId = 0;
	this->mEboId = 0;
	this->mProgramId = 0;
	this->isOpenDebugInfo = false;
	this->isOpenShaderDebugInfo = false;
}

AnimationPlayer::~AnimationPlayer()
{
}

void AnimationPlayer::setWidget(QWidget *w)
{
	this->mWidget = w;
	this->mWidth = w->width();
	this->mHeight = w->height();
}

void AnimationPlayer::setHWND(HWND &h)
{
	this->mHWND = h;
}

void AnimationPlayer::setFrameRate(int fr)
{
	this->mFrameRate = fr;
}

void AnimationPlayer::setImageArray(QVector<QImage> &v)
{
	this->mImageArray = v;
}

void AnimationPlayer::setOpenDebugInfo(bool b)
{
	this->isOpenDebugInfo = b;
}

void AnimationPlayer::setOpenShaderDebugInfo(bool b)
{
	this->isOpenShaderDebugInfo = b;
}

void AnimationPlayer::stop()
{
	isRun = false;
	mWidget->close();
}

int AnimationPlayer::setupPixelFormat(HDC hDC)
{
	int pixelIdx;
	// get the appropriate pixel format 
	pixelIdx = ChoosePixelFormat(hDC, &pfd);
	if (pixelIdx == 0) {
		printf("ChoosePixelFormat() failed:  Cannot find format specified.");
		return 0;
	}

	/* set the pixel format */
	if (SetPixelFormat(hDC, pixelIdx, &pfd) == FALSE) {
		printf("SetPixelFormat() failed:  Cannot set format specified.");
		return 0;
	}

	return pixelIdx;
}

void AnimationPlayer::GLInfo() {
	printf("InitGLEW SUCCESS\n");
	printf("\nOpenGL vendor: %s\n", glGetString(GL_VENDOR));
	printf("OpenGL renderer: %s\n", glGetString(GL_RENDERER));
	printf("OpenGL version: %s\n", glGetString(GL_VERSION));
}

void AnimationPlayer::timer_start(FILETIME *start)
{
	GetSystemTimeAsFileTime(start);
}

int64_t AnimationPlayer::timer_elapsed_msec(FILETIME *start)
{
	FILETIME now;
	GetSystemTimeAsFileTime(&now);
	int64_t elapsed = ((((int64_t)now.dwHighDateTime) << 32) + (int64_t)now.dwLowDateTime) -
		((((int64_t)start->dwHighDateTime) << 32) + (int64_t)start->dwLowDateTime);
	return elapsed / 10000;
}

int64_t AnimationPlayer::timer_elapsed_usec(FILETIME *start)
{
	FILETIME now;
	GetSystemTimeAsFileTime(&now);
	int64_t elapsed = ((((int64_t)now.dwHighDateTime) << 32) + (int64_t)now.dwLowDateTime) -
		((((int64_t)start->dwHighDateTime) << 32) + (int64_t)start->dwLowDateTime);
	return elapsed / 10;
}

GLuint AnimationPlayer::initShader(const char* shaderSource, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);
	if (isOpenShaderDebugInfo)
	{
		int  success;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			printf("COMPILE SHADER ERRPR,%s\n", std::string(infoLog).c_str());
		}
		else
		{
			printf("COMPILE SHADER SUCCESS\n");
		}
	}
	return shader;
}

GLuint AnimationPlayer::initShaderProgram(const char* vertexShaderSource, const char* fragShaderSource)
{
	GLuint vertexShader = initShader(vertexShaderSource, GL_VERTEX_SHADER);
	GLuint fragShader = initShader(fragShaderSource, GL_FRAGMENT_SHADER);
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);
	if (isOpenShaderDebugInfo)
	{
		int  success;
		char infoLog[512];
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(program, 512, NULL, infoLog);
			printf("LINK PROGRAM ERROR %s\n", std::string(infoLog).c_str());
		}
		else
		{
			printf("LINK PROGRAM SUCCESS\n");
		}
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);
	return program;
}

bool AnimationPlayer::initGLEW()
{
	bool ret = true;
	GLenum err = glewInit();
	if (isOpenDebugInfo && err != GLEW_OK)
		printf("GLEW Error: %s\n", glewGetErrorString(err));
	if (isOpenDebugInfo)
		GLInfo();
	return ret;
}

void AnimationPlayer::initGL()
{
	mWinDC = GetDC(mHWND);
	setupPixelFormat(mWinDC);
	mWinRC = wglCreateContext(mWinDC);
	wglMakeCurrent(mWinDC, mWinRC);
	bool ret = initGLEW();
	mProgramId = initShaderProgram(vertexShader, fragmentShader);

	glGenVertexArrays(1, &mVaoId);
	glBindVertexArray(mVaoId);

	glGenBuffers(1, &mEboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &mVboId);
	glBindBuffer(GL_ARRAY_BUFFER, mVboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, (void*)(sizeof(GL_FLOAT) * 3));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void AnimationPlayer::initImageTextureArray()
{
	for (int i = 0; i < mImageArray.size(); i++)
	{
		QImage image = mImageArray.at(i);
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(),
			0, GL_RGBA, GL_UNSIGNED_BYTE, image.constBits());
		glBindTexture(GL_TEXTURE_2D, 0);
		mImageTextureArray.push_back(texture);
	}
}

void AnimationPlayer::run()
{
	initGL();
	initImageTextureArray();
	glViewport(0, 0, mWidth, mHeight);
	glUseProgram(mProgramId);
	float mBgColor = 0.0f;
	int textureIndex = 0;
	while (isRun)
	{
		FILETIME renderTimer;
		timer_start(&renderTimer);
		if (mBgColor < 0.5)
			mBgColor = 1.0f;
		else
			mBgColor = 0.0f;
		glClearColor(mBgColor, mBgColor, mBgColor, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(mVaoId);
		GLuint texId = mImageTextureArray.at(textureIndex);
		glBindTexture(GL_TEXTURE_2D, texId);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glFinish();
		int renderTime = timer_elapsed_msec(&renderTimer);
		int frameTime = 1000 / mFrameRate;
		int sleepTime = frameTime - renderTime;
		if (sleepTime < 0)
			sleepTime = 0;
		Sleep(sleepTime);
		//printf("renderTime = %d, frameTime = %d, sleep time = %d\n", renderTime, frameTime, frameTime - renderTime);
		SwapBuffers(mWinDC);
		textureIndex++;
		if (textureIndex > mImageTextureArray.size() - 1)
			textureIndex = 0;
	}
}