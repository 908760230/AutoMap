#pragma once

#include <QWidget>
#include <qtextbrowser.h>

class OutputWidget  : public QWidget
{
	Q_OBJECT

public:
	OutputWidget(QWidget *parent = nullptr);
	~OutputWidget();

private:
	QTextBrowser* m_textBrownser = nullptr;
};
