#include "ToolWidget.h"
#include "qlayout.h"
#include <qlabel.h>
#include <qscrollarea.h>

ToolWidget::ToolWidget()
{
	m_btnElevation = new QPushButton("elevation");
	connect(m_btnElevation, &QPushButton::clicked, this, &ToolWidget::showElevationWidget);

	m_elevationWidget = new QWidget;
	QVBoxLayout* elevationLayout = new QVBoxLayout(m_elevationWidget);
	elevationLayout->setContentsMargins(0, 0, 0, 0);
	elevationLayout->setSpacing(0);
	elevationLayout->addWidget(new QLabel("Seed"));
	m_lineEditSeed = new QLineEdit;
	elevationLayout->addWidget(m_lineEditSeed);

	elevationLayout->addWidget(new QLabel("Island"));
	m_sliderIsland = new QSlider;
	m_sliderIsland->setOrientation(Qt::Horizontal);
	elevationLayout->addWidget(m_sliderIsland);

	elevationLayout->addWidget(new QLabel("noisyCoastlines"));
	m_sliderNoisyCoastlines = new QSlider;
	m_sliderNoisyCoastlines->setOrientation(Qt::Horizontal);
	elevationLayout->addWidget(m_sliderNoisyCoastlines);

	elevationLayout->addWidget(new QLabel("hillHeight"));
	m_sliderHillHeight = new QSlider;
	m_sliderHillHeight->setOrientation(Qt::Horizontal);
	elevationLayout->addWidget(m_sliderHillHeight);

	elevationLayout->addWidget(new QLabel("mountainJagged"));
	m_sliderMountainJagged = new QSlider;
	m_sliderMountainJagged->setOrientation(Qt::Horizontal);
	elevationLayout->addWidget(m_sliderMountainJagged);

	elevationLayout->addWidget(new QLabel("mountainSharpness"));
	m_sliderMountainSharpness = new QSlider;
	m_sliderMountainSharpness->setOrientation(Qt::Horizontal);
	elevationLayout->addWidget(m_sliderMountainSharpness);

	elevationLayout->addWidget(new QLabel("oceanDepth"));
	m_sliderOceanDepth = new QSlider;
	m_sliderOceanDepth->setOrientation(Qt::Horizontal);
	elevationLayout->addWidget(m_sliderOceanDepth);

	m_btnBiomes = new QPushButton("biomes");
	connect(m_btnBiomes, &QPushButton::clicked, this, &ToolWidget::showBiomesWidget);
	m_biomesWidget = new QWidget;
	QVBoxLayout* biomesLayout = new QVBoxLayout(m_biomesWidget);
	biomesLayout->setContentsMargins(0, 0, 0, 0);
	biomesLayout->setSpacing(0);
	biomesLayout->addWidget(new QLabel("windAngleDeg"));
	m_sliderWindAngleDeg = new QSlider;
	m_sliderWindAngleDeg->setOrientation(Qt::Horizontal);
	biomesLayout->addWidget(m_sliderWindAngleDeg);

	biomesLayout->addWidget(new QLabel("raininess"));
	m_sliderRaininess = new QSlider;
	m_sliderRaininess->setOrientation(Qt::Horizontal);
	biomesLayout->addWidget(m_sliderRaininess);

	biomesLayout->addWidget(new QLabel("rainShadow"));
	m_sliderRainShadow = new QSlider;
	m_sliderRainShadow->setOrientation(Qt::Horizontal);
	biomesLayout->addWidget(m_sliderRainShadow);

	biomesLayout->addWidget(new QLabel("evaporation"));
	m_sliderEvaporation = new QSlider;
	m_sliderEvaporation->setOrientation(Qt::Horizontal);
	biomesLayout->addWidget(m_sliderEvaporation);

	m_btnRivers = new QPushButton("rivers");
	connect(m_btnRivers, &QPushButton::clicked, this, &ToolWidget::showRiversWidget);

	m_riversWidget = new QWidget;
	QVBoxLayout* riversLayout = new QVBoxLayout(m_riversWidget);
	riversLayout->setContentsMargins(0, 0, 0, 0);
	riversLayout->setSpacing(0);
	riversLayout->addWidget(new QLabel("lgMinFlow"));
	m_sliderLgMinFlow = new QSlider;
	m_sliderLgMinFlow->setOrientation(Qt::Horizontal);
	riversLayout->addWidget(m_sliderLgMinFlow);

	riversLayout->addWidget(new QLabel("lgRiverWidth"));
	m_sliderLgRiverWidth = new QSlider;
	m_sliderLgRiverWidth->setOrientation(Qt::Horizontal);
	riversLayout->addWidget(m_sliderLgRiverWidth);

	riversLayout->addWidget(new QLabel("flow"));
	m_sliderFlow = new QSlider;
	m_sliderFlow->setOrientation(Qt::Horizontal);
	riversLayout->addWidget(m_sliderFlow);

	m_btnRender = new QPushButton("render");
	connect(m_btnRender, &QPushButton::clicked, this, &ToolWidget::showRenderWidget);

	m_renderWidget = new QWidget;
	QVBoxLayout* renderLayout = new QVBoxLayout(m_renderWidget);
	renderLayout->setContentsMargins(0, 0, 0, 0);
	renderLayout->setSpacing(0);
	renderLayout->addWidget(new QLabel("zoom"));
	m_sliderZoom = new QSlider;
	m_sliderZoom->setOrientation(Qt::Horizontal);
	renderLayout->addWidget(m_sliderZoom);

	renderLayout->addWidget(new QLabel("x"));
	m_sliderX = new QSlider;
	m_sliderX->setOrientation(Qt::Horizontal);
	renderLayout->addWidget(m_sliderX);

	renderLayout->addWidget(new QLabel("y"));
	m_sliderY = new QSlider;
	m_sliderY->setOrientation(Qt::Horizontal);
	renderLayout->addWidget(m_sliderY);

	renderLayout->addWidget(new QLabel("lightAngleDeg"));
	m_sliderLightAngleDeg = new QSlider;
	m_sliderLightAngleDeg->setOrientation(Qt::Horizontal);
	renderLayout->addWidget(m_sliderLightAngleDeg);

	renderLayout->addWidget(new QLabel("slope"));
	m_sliderSlope = new QSlider;
	m_sliderSlope->setOrientation(Qt::Horizontal);
	renderLayout->addWidget(m_sliderSlope);

	renderLayout->addWidget(new QLabel("flat"));
	m_sliderFlat = new QSlider;
	m_sliderFlat->setOrientation(Qt::Horizontal);
	renderLayout->addWidget(m_sliderFlat);

	renderLayout->addWidget(new QLabel("ambient"));
	m_sliderAmbient = new QSlider;
	m_sliderAmbient->setOrientation(Qt::Horizontal);
	renderLayout->addWidget(m_sliderAmbient);

	renderLayout->addWidget(new QLabel("overhead"));
	m_sliderOverhead = new QSlider;
	m_sliderOverhead->setOrientation(Qt::Horizontal);
	renderLayout->addWidget(m_sliderOverhead);

	renderLayout->addWidget(new QLabel("tiltDeg"));
	m_sliderTiltDeg = new QSlider;
	m_sliderTiltDeg->setOrientation(Qt::Horizontal);
	renderLayout->addWidget(m_sliderTiltDeg);

	renderLayout->addWidget(new QLabel("rotateDeg"));
	m_sliderRotateDeg = new QSlider;
	m_sliderRotateDeg->setOrientation(Qt::Horizontal);
	renderLayout->addWidget(m_sliderRotateDeg);

	renderLayout->addWidget(new QLabel("mountainHeight"));
	m_sliderMountainHeight = new QSlider;
	m_sliderMountainHeight->setOrientation(Qt::Horizontal);
	renderLayout->addWidget(m_sliderMountainHeight);

	renderLayout->addWidget(new QLabel("outlineDepth "));
	m_sliderOutlineDepth = new QSlider;
	m_sliderOutlineDepth->setOrientation(Qt::Horizontal);
	renderLayout->addWidget(m_sliderOutlineDepth);

	renderLayout->addWidget(new QLabel("outlineStrength"));
	m_sliderOutlineStrength = new QSlider;
	m_sliderOutlineStrength->setOrientation(Qt::Horizontal);
	renderLayout->addWidget(m_sliderOutlineStrength);

	renderLayout->addWidget(new QLabel("outlineThreshold"));
	m_sliderOutlineThreshold = new QSlider;
	m_sliderOutlineThreshold->setOrientation(Qt::Horizontal);
	renderLayout->addWidget(m_sliderOutlineThreshold);

	renderLayout->addWidget(new QLabel("outlineCoast"));
	m_sliderOutlineCoast = new QSlider;
	m_sliderOutlineCoast->setOrientation(Qt::Horizontal);
	renderLayout->addWidget(m_sliderOutlineCoast);

	renderLayout->addWidget(new QLabel("outlineWater"));
	m_sliderOutlineWater = new QSlider;
	m_sliderOutlineWater->setOrientation(Qt::Horizontal);
	renderLayout->addWidget(m_sliderOutlineWater);

	renderLayout->addWidget(new QLabel("biomeColors"));
	m_sliderBiomeColors = new QSlider;
	m_sliderBiomeColors->setOrientation(Qt::Horizontal);
	renderLayout->addWidget(m_sliderBiomeColors);

	QWidget* scrollAreaWidget = new  QWidget;
	QVBoxLayout* scrollAreaLayout = new QVBoxLayout(scrollAreaWidget);
	scrollAreaLayout->setContentsMargins(0, 0, 0, 0);
	scrollAreaLayout->setSpacing(0);
	scrollAreaLayout->addWidget(m_btnElevation);
	scrollAreaLayout->addWidget(m_elevationWidget);
	scrollAreaLayout->addWidget(m_btnBiomes);
	scrollAreaLayout->addWidget(m_biomesWidget);
	scrollAreaLayout->addWidget(m_btnRivers);
	scrollAreaLayout->addWidget(m_riversWidget);
	scrollAreaLayout->addWidget(m_btnRender);
	scrollAreaLayout->addWidget(m_renderWidget);
	QScrollArea* scrollArea = new QScrollArea;
	scrollArea->setWidget(scrollAreaWidget);
	scrollArea->setWidgetResizable(true);

	QVBoxLayout* toolLayout = new QVBoxLayout(this);
	toolLayout->addWidget(scrollArea);
}

ToolWidget::~ToolWidget()
{
}
void ToolWidget::showElevationWidget() {
	bool value = m_elevationWidget->isHidden();
	m_elevationWidget->setHidden(!value);
}

void ToolWidget::showBiomesWidget()
{
	bool value = m_biomesWidget->isHidden();
	m_biomesWidget->setHidden(!value);
}
void ToolWidget::showRiversWidget() {
	bool value = m_riversWidget->isHidden();
	m_riversWidget->setHidden(!value);
}
void ToolWidget::showRenderWidget() {
	bool value = m_renderWidget->isHidden();
	m_renderWidget->setHidden(!value);
}
