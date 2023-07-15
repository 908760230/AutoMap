#include "ToolWidget.h"
#include "qlayout.h"
#include <qlabel.h>

ToolWidget::ToolWidget()
{
	QWidget* elevationWidget = new QWidget;
	QVBoxLayout* elevationLayout = new QVBoxLayout(elevationWidget);
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

	QWidget* biomesWidget = new QWidget;
	QVBoxLayout* biomesLayout = new QVBoxLayout(biomesWidget);
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

	//biomesLayout->addWidget(new QLabel("hillHeight"));
	//m_sliderHillHeight = new QSlider;
	//m_sliderHillHeight->setOrientation(Qt::Horizontal);
	//biomesLayout->addWidget(m_sliderHillHeight);

	QVBoxLayout* toolLayout = new QVBoxLayout(this);
	toolLayout->addWidget(elevationWidget);
	toolLayout->addWidget(biomesWidget);
}

ToolWidget::~ToolWidget()
{
}
