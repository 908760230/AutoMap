#include "DetailDock.h"
#include <qlabel.h>
#include <qwidget.h>
#include <qlayout.h>

DetailDock::DetailDock()
{
	QWidget* mainWidget = new QWidget;
	QVBoxLayout* vbl = new  QVBoxLayout(mainWidget);
	QLabel* lable = new QLabel("DetailWidget");
	vbl->addWidget(lable);

	setWidget(mainWidget);
}

DetailDock::~DetailDock()
{
}
