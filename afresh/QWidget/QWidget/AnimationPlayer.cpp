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

AnimationPlayer::AnimationPlayer(QWidget *w, HWND h)
{
	this->hwnd = h;
	this->mWidget = w;
	this->isRun = true;
	this->winDC = NULL;
	this->winRC = NULL;
	this->frameRate = 10;
}

AnimationPlayer::~AnimationPlayer()
{
}

void AnimationPlayer::setHWND(HWND &h)
{
	this->hwnd = h;
}

void AnimationPlayer::setFrameRate(int fr)
{
	this->frameRate = fr;
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

bool AnimationPlayer::initGLEW()
{
	bool ret = true;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		printf("GLEW Error: %s\n", glewGetErrorString(err));
		exit(0);
	}
	GLInfo();
	return ret;
}

void AnimationPlayer::initGL()
{
	winDC = GetDC(hwnd);
	setupPixelFormat(winDC);
	winRC = wglCreateContext(winDC);
	wglMakeCurrent(winDC, winRC);
	bool ret = initGLEW();
	printf("initGLEW = %d\n", ret);
	//wglSwapIntervalEXT(10);
}

void AnimationPlayer::run()
{
	initGL();
	float mBgColor = 0.0f;
	while (isRun)
	{
		FILETIME renderTimer;
		timer_start(&renderTimer);

		glClearColor(mBgColor, mBgColor, mBgColor, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		if (mBgColor < 0.5)
			mBgColor = 1.0f;
		else
			mBgColor = 0.0f;
		glFinish();

		int renderTime = timer_elapsed_msec(&renderTimer);
		int frameTime = 1000 / frameRate;
		int sleepTime = frameTime - renderTime;
		if (sleepTime < 0)
			sleepTime = 0;
		//Sleep(sleepTime);
		printf("renderTime = %d, frameTime = %d, sleep time = %d\n", renderTime, frameTime, frameTime - renderTime);
		SwapBuffers(winDC);
	}
}