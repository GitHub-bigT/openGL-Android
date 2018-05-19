#include <iostream>
#include <QVector>
#include <QApplication>
#include <QWindow>
#include <QWidget>
#include <QImage>
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

QVector<QImage> createQImageArray()
{
	QVector<QImage> v;
	for (int i = 0; i < 30; i++)
	{
		QString name = QString::asprintf("video/%02d.png", i);
		QImage image(name);
		v.push_back(image);
	}
	return v;
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QWidget *w = createQWidget();
	HWND hwnd = (HWND)w->winId();
	QVector<QImage> v = createQImageArray();
	//AnimationPlayer ap(w, hwnd, 30, v);
	AnimationPlayer ap;
	ap.setWidget(w);
	ap.setHWND(hwnd);
	ap.setFrameRate(60);
	ap.setImageArray(v);
	//ap.setOpenDebugInfo(true);
	//ap.setOpenShaderDebugInfo(true);
	ap.start();
	//Sleep(10 * 1000);
	//ap.stop();

	return a.exec();
}