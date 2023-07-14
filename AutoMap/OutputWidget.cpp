#include "OutputWidget.h"
#include <qlayout.h>

OutputWidget::OutputWidget(QWidget *parent)
	: QWidget(parent)
{
	m_textBrownser = new QTextBrowser;
	m_textBrownser->append("this is a twersdsfdsfsdfsdfdsfdsf!");
	QVBoxLayout* vBoxLayout = new QVBoxLayout;
	vBoxLayout->setContentsMargins(0, 0, 0, 0);
	vBoxLayout->addWidget(m_textBrownser);
	setLayout(vBoxLayout);
}

OutputWidget::~OutputWidget()
{}
