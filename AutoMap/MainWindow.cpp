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
#include <qdebug.h>

#include "TriangleMesh.h"

MainWindow::MainWindow()
{
	QAction* actionLoadDataFile = new QAction("Load Data File");
	connect(actionLoadDataFile, &QAction::triggered, this, &MainWindow::LoadDataFile);

	QMenu* menuFile = new  QMenu(tr("File"));
	menuFile->addAction(actionLoadDataFile);
	menuBar()->addMenu(menuFile);

	QAction* actionToolWindow = new QAction("ToolWindow");
	QAction* actionOutputWidget = new QAction("OutputWindow");
	QMenu* menuWindow = new QMenu(tr("Window"));
	menuWindow->addAction(actionToolWindow);
	menuWindow->addAction(actionOutputWidget);
	menuBar()->addMenu(menuWindow);

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
	QVector<unsigned short> fileData((size + 1 )/ 2);
	auto byteArray = file.readAll();
	memcpy(fileData.data(), byteArray.data(), size);
	file.close();

	auto [points, numExteriorBoundaryPoints, numInteriorBoundaryPoints, numMountainPoints] = parseData(fileData);
	MeshInitializer meshInit = TriangleMesh::addGhostStructure(points, Delaunator(points), numExteriorBoundaryPoints);

	TriangleMesh mesh(meshInit);
	qDebug() << "triangles = " << mesh.numTriangles <<" regions = " << mesh.numRegions;

	mesh.is_boundary_t.resize(mesh.numTriangles);
	for (int t = 0; t < mesh.numTriangles; t++) {
		mesh.is_boundary_t[t] = 0;
		auto data = mesh.r_around_t(t);
		for (auto number : data) {
			if (mesh.is_boundary_r(number)) {
				mesh.is_boundary_t[t] = 1;
				break;
			}
		}
	}

	mesh.length_s.resize(mesh.numSides);
	for (unsigned int s = 0; s < mesh.numSides; s++) {
		unsigned int r1 = mesh.r_begin_s(s),
			r2 = mesh.r_end_s(s);
		double dx = mesh.x_of_r(r1) - mesh.x_of_r(r2),
			dy = mesh.y_of_r(r1) - mesh.y_of_r(r2);
		mesh.length_s[s] = sqrtf(dx * dx + dy * dy);
	}

	// NOTE: these are all contigious so it could be shortened to a range
	// (they were not contiguous in earlier versions of mapgen4, so that's
	// why it's an array of indices)
	QVector<unsigned int> r_peaks(numMountainPoints,0);
	for (int i = 0; i < numMountainPoints; i++) {
		r_peaks[i] = i + numExteriorBoundaryPoints + numInteriorBoundaryPoints;
	}

	// Poisson disc chooses mountain regions but we actually need mountain triangles
	// so we'll just pick one neighboring triangle for each region
	QVector<int> t_peaks;
	for (auto r : r_peaks) {
		t_peaks.push_back(mesh.t_inner_s(mesh.s_of_r[r]));
	}

	//return { mesh, t_peaks };
}

std::tuple<QVector<QPointF>, unsigned int, unsigned int,unsigned int> MainWindow::parseData(const QVector<unsigned short>& data)
{
	auto rescale = [](double value)-> double {
		double after[2] = { -100,1000 + 100 };
		double before[2] = { 0, ((size_t)1 << 16) - 1 };
		return (value - before[0]) / (before[1] - before[0]) * (after[1] - after[0]) + after[0];
	};
	unsigned int numExteriorBoundaryPoints = data[0];
	unsigned int numInteriorBoundaryPoints = data[1];
	unsigned int numMountainPoints = data[2];
	QVector<QPointF> points;
	for (int i = 3; i < data.size(); i += 2) {
		double x = rescale(data[i]);
		double y = rescale(data[i + 1]);
		points.push_back(QPointF(x,y));
	}
	return { points,
		numExteriorBoundaryPoints,
		numInteriorBoundaryPoints,
		numMountainPoints, };
}
