#include <iostream>
#include <QApplication>
#include <QWindow>
#include <QWidget>
//#include <QOpenGLWidget>
#include "AnimationPlayer.h"

QWidget* createQWidget()
{
	QWidget *w = new QWidget;
	w->setGeometry(100, 100, 800, 600);
	QSurfaceFormat format;
	//format.setSwapInterval(10);
	//format.setDepthBufferSize(0);
	//format.setStencilBufferSize(0);
	format.setVersion(4, 3);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
	/*
	format.setSamples(4);
	format.setMajorVersion(4);
	format.setMinorVersion(0);

	format.setProfile(QSurfaceFormat::CompatibilityProfile);
	*/
	QSurfaceFormat::setDefaultFormat(format);
	w->show();
	return w;
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QWidget *w = createQWidget();
	HWND hwnd = (HWND)w->winId();

	AnimationPlayer ap(w, hwnd);
	ap.start();
	Sleep(10 * 1000);
	ap.stop();

	return a.exec();
}