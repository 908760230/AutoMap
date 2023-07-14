#include "MainWindow.h"
#include "DisplayWidget.h"
#include "DetailDock.h"
#include "ContentDock.h"

#include <qmenu.h>
#include <qmenubar.h>
#include <qlayout.h>
#include <qdockwidget.h>
#include <qboxlayout.h>
#include <qfile.h>
#include <qdatastream.h>

#include "TriangleMesh.h"

MainWindow::MainWindow()
{
	QAction* actionLoadDataFile = new QAction("Load Data File");
	connect(actionLoadDataFile, &QAction::triggered, this, &MainWindow::LoadDataFile);

	QMenu* menuFile = new  QMenu(tr("File"));
	menuFile->addAction(actionLoadDataFile);
	menuBar()->addMenu(menuFile);
	

	DisplayWidget* displayWidget = new DisplayWidget;
	setCentralWidget(displayWidget);

	DetailDock* detailDock = new  DetailDock;
	ContentDock* contentDock = new ContentDock;

	addDockWidget(Qt::BottomDockWidgetArea, contentDock);
	addDockWidget(Qt::RightDockWidgetArea, detailDock, Qt::Orientation::Vertical);
}

MainWindow::~MainWindow()
{
}

void MainWindow::LoadDataFile()
{
	QFile file("points-5.5.data");
	if (!file.open(QIODevice::ReadOnly))  return;
	auto size = file.size();
	QVector<short> fileData((size + 1 )/ 2);
	auto byteArray = file.readAll();
	memcpy(fileData.data(), byteArray.data(), size);
	file.close();

	auto [points, numExteriorBoundaryPoints, numInteriorBoundaryPoints, numMountainPoints] = parseData(fileData);
	MeshInitializer meshInit = TriangleMesh::addGhostStructure(points, Delaunator(points), numExteriorBoundaryPoints);
}

std::tuple<QVector<QPoint>, unsigned int, unsigned int,unsigned int> MainWindow::parseData(const QVector<short>& data)
{
	auto rescale = [](short value)-> unsigned int {
		int before[2] = { -100,1000 + 100 };
		int after[2] = { 0 (1 << 16) - 1 };
		return (value - before[0]) / (before[1] - before[0]) * (after[1] - after[0]) + after[0];
	};
	unsigned int numExteriorBoundaryPoints = data[0];
	unsigned int numInteriorBoundaryPoints = data[1];
	unsigned int numMountainPoints = data[2];
	QVector<QPoint> points;
	for (int i = 3; i < data.size(); i += 2) {
		unsigned int x = rescale(data[i]);
		unsigned int y = rescale(data[i + 1]);
		points.push_back(QPoint(x,y));
	}
	return { points,
		numExteriorBoundaryPoints,
		numInteriorBoundaryPoints,
		numMountainPoints, };
}
