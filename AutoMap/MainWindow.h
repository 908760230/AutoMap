#pragma once
#include <qmainwindow.h>
#include <tuple>
class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow();
	~MainWindow();

public:
	void LoadDataFile();
	std::tuple<QVector<QPointF>, unsigned int, unsigned int, unsigned int> parseData(const QVector<unsigned short>& data);
private:
};

