#include "ContentDock.h"
#include <qlayout.h>

ContentDock::ContentDock()
{
	m_outputWidget = new OutputWidget;
	setWidget(m_outputWidget);
}

ContentDock::~ContentDock()
{
}
