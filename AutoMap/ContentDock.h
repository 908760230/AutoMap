#pragma once
#include <qdockwidget.h>
#include "OutputWidget.h"

class ContentDock : public QDockWidget
{
public:
	ContentDock();
	~ContentDock();

private:
	OutputWidget* m_outputWidget = nullptr;
};

