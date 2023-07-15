#include "DisplayWidget.h"
#include <qlabel.h>

DisplayWidget::DisplayWidget(QWidget* parent):QOpenGLWidget(parent)
{
	setFixedSize(800, 600);
	//QLabel* lable = new QLabel("DisplayWidget", this);
}

DisplayWidget::~DisplayWidget()
{
}

void DisplayWidget::initializeGL()
{
	initializeOpenGLFunctions();
}

void DisplayWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
}

void DisplayWidget::paintGL()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //清空屏幕所用的颜色,参数分别为红、绿、蓝以及透明度
	glClear(GL_COLOR_BUFFER_BIT); //清空屏幕的颜色缓冲
}
