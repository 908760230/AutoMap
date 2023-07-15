#pragma once

#include <qwidget.h>
#include <qslider.h>
#include <qlineedit.h>

class ToolWidget : public QWidget {
public:
	ToolWidget();
	~ToolWidget();
private:
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
	//QSlider* m_sliderIsland = nullptr;
};