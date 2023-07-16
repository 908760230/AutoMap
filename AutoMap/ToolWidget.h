#pragma once

#include <qwidget.h>
#include <qslider.h>
#include <qlineedit.h>
#include <qpushbutton.h>

class ToolWidget : public QWidget {
public:
	ToolWidget();
	~ToolWidget();

public slots:
	void showElevationWidget();
	void showBiomesWidget();
	void showRiversWidget();
	void showRenderWidget();

private:
	QPushButton* m_btnElevation = nullptr;
	QPushButton* m_btnBiomes = nullptr;
	QPushButton* m_btnRivers = nullptr;
	QPushButton* m_btnRender = nullptr;

	QWidget* m_elevationWidget = nullptr;
	QWidget* m_biomesWidget = nullptr;
	QWidget* m_riversWidget = nullptr;
	QWidget* m_renderWidget = nullptr;

	QLineEdit* m_lineEditSeed = nullptr;
	QSlider* m_sliderIsland = nullptr;
	QSlider* m_sliderNoisyCoastlines = nullptr;
	QSlider* m_sliderHillHeight = nullptr;
	QSlider* m_sliderMountainJagged = nullptr;
	QSlider* m_sliderMountainSharpness = nullptr;
	QSlider* m_sliderOceanDepth = nullptr;
	QSlider* m_sliderWindAngleDeg = nullptr;
	QSlider* m_sliderRaininess = nullptr;
	QSlider* m_sliderRainShadow = nullptr;
	QSlider* m_sliderEvaporation = nullptr;
	QSlider* m_sliderLgMinFlow = nullptr;
	QSlider* m_sliderLgRiverWidth = nullptr;
	QSlider* m_sliderFlow = nullptr;
	QSlider* m_sliderZoom = nullptr;
	QSlider* m_sliderX = nullptr;
	QSlider* m_sliderY = nullptr;
	QSlider* m_sliderLightAngleDeg = nullptr;
	QSlider* m_sliderSlope = nullptr;
	QSlider* m_sliderFlat = nullptr;
	QSlider* m_sliderAmbient = nullptr;
	QSlider* m_sliderOverhead = nullptr;
	QSlider* m_sliderTiltDeg = nullptr;
	QSlider* m_sliderRotateDeg = nullptr;
	QSlider* m_sliderMountainHeight = nullptr;
	QSlider* m_sliderOutlineDepth = nullptr;
	QSlider* m_sliderOutlineStrength = nullptr;
	QSlider* m_sliderOutlineThreshold = nullptr;
	QSlider* m_sliderOutlineCoast = nullptr;
	QSlider* m_sliderOutlineWater = nullptr;
	QSlider* m_sliderBiomeColors = nullptr;
};