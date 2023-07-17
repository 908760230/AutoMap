#pragma once
#include <qopenglwidget.h>
#include <qopenglextrafunctions.h>
#include <qopenglfunctions.h>

class DisplayWidget :public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	DisplayWidget(QWidget* parent = nullptr);
	~DisplayWidget();
	static int getWidth();
	static int getHeight();
protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;

	void paintGL() override;
private:

};

