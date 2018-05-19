#include "AnimationPlayer.h"

static    PIXELFORMATDESCRIPTOR pfd =
{
	sizeof(PIXELFORMATDESCRIPTOR),    //上述格式描述符的大小
	1,                                //版本号
	PFD_DRAW_TO_WINDOW |              //格式支持窗口
	PFD_SUPPORT_OPENGL |              //格式必须支持OpenGL
	PFD_DOUBLEBUFFER,                 //必须支持双缓冲
	PFD_TYPE_RGBA,                    //申请RGBA 格式
	24,                               //选定色彩深度
	0, 0, 0, 0, 0, 0, 0, 0,           //忽略RGBA
	0,                                //无累加缓存
	0, 0, 0, 0,                       //忽略聚集位
	32,                               //32位Z-缓存(深度缓存)
	0,                                //无蒙板缓存
	0,                                //无辅助缓存
	PFD_MAIN_PLANE,                   //主绘图层
	0,                                //Reserved
	0, 0, 0                           //忽略层遮罩
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