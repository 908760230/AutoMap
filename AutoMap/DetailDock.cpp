#include "DetailDock.h"
#include <qlabel.h>
#include <qwidget.h>
#include <qlayout.h>


#include "ToolWidget.h"

DetailDock::DetailDock()
{
	ToolWidget* mainWidget = new ToolWidget;
	setWidget(mainWidget);
}

DetailDock::~DetailDock()
{
}
