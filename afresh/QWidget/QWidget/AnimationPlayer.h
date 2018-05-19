#pragma once
#include <QWidget>
#include <QThread>
#include <windows.h>
#include <stdint.h>
#include "GL/glew.h"
#include "GL/wglew.h"

class AnimationPlayer : public QThread
{
public:
	AnimationPlayer(QWidget *w, HWND h);
	~AnimationPlayer();
	void setHWND(HWND &h);
	void setFrameRate(int fr);
	void stop();

private:
	QWidget *mWidget;
	HWND hwnd;
	HDC winDC;
	HGLRC winRC;
	bool isRun;
	int frameRate;
	int setupPixelFormat(HDC hDC);
	bool initGLEW();
	void GLInfo();
	void initGL();
	void run();
	void timer_start(FILETIME *start);
	int64_t timer_elapsed_msec(FILETIME *start);
	int64_t timer_elapsed_usec(FILETIME *start);
};
