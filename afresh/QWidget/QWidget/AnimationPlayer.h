#pragma once
#include <QWidget>
#include <QThread>
#include <QImage>
#include <QVector>
#include <windows.h>
#include <stdint.h>
#include "GL/glew.h"
#include "GL/wglew.h"

class AnimationPlayer : public QThread
{
public:
	AnimationPlayer(QWidget *w, HWND h, int fr, QVector<QImage> &v);
	AnimationPlayer();
	~AnimationPlayer();
	void setWidget(QWidget *w);
	void setHWND(HWND &h);
	void setFrameRate(int fr);
	void setImageArray(QVector<QImage> &v);
	void setOpenDebugInfo(bool b);
	void setOpenShaderDebugInfo(bool b);
	void stop();

private:
	QWidget *mWidget;
	HWND mHWND;
	HDC mWinDC;
	HGLRC mWinRC;
	GLuint mProgramId;
	GLuint mVaoId, mVboId, mEboId;
	QVector<QImage> mImageArray;
	QVector<GLuint> mImageTextureArray;
	int mFrameRate, mWidth, mHeight;
	bool isRun, isOpenDebugInfo, isOpenShaderDebugInfo;

private:
	bool initGLEW();
	void initGL();
	void initImageTextureArray();
	GLuint initShader(const char* shaderSource, GLenum shaderType);
	GLuint initShaderProgram(const char* vertexShaderSource, const char* fragShaderSource);
	void GLInfo();
	int setupPixelFormat(HDC hDC);
	void run();
	void timer_start(FILETIME *start);
	int64_t timer_elapsed_msec(FILETIME *start);
	int64_t timer_elapsed_usec(FILETIME *start);
};
