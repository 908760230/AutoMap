#pragma once

#include <glm/glm.hpp>
#include <qvector.h>

#include "TriangleMesh.h"
#include <qopenglshaderprogram.h>

class Render {
public:
	Render(TriangleMesh &mesh);


	glm::vec2 screenToWorld(double x, double y);

	void drawLand();
	void drawRiver();
	void drawDrape();
private:
	int m_numRiverTriangles = 0;
	glm::mat4 m_TopDown;
	glm::mat4 m_inverse_projection;
	QVector<double> a_quad_xy;
	QVector<double> a_quad_em;
	QVector<int>    quad_elements;
	QVector<double> a_river_xyuv;

	QOpenGLShaderProgram m_shaderProgramRiver;
	QOpenGLShaderProgram m_shaderProgramLand;
	QOpenGLShaderProgram m_shaderProgramDepth;
	QOpenGLShaderProgram m_shaderProgramDrape;
	QOpenGLShaderProgram m_shaderProgramFinal;
};