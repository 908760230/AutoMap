#pragma once

#include <glm/glm.hpp>
#include <qvector.h>

#include "TriangleMesh.h"
#include "Geometry.h"
#include <qopenglshaderprogram.h>

class Render {
public:
	Render(const TriangleMesh &mesh);
	~Render();
	glm::vec2 screenToWorld(double x, double y);

	void setMeshGeometry(const TriangleMesh& mesh);

	void drawLand();
	void drawRiver();
	void drawDrape();
private:
	int m_numRiverTriangles = 0;
	glm::mat4 m_TopDown;
	glm::mat4 m_projection = glm::mat4(1.0);
	glm::mat4 m_inverse_projection = glm::mat4(1.0);
	QVector<double> a_quad_xy;
	QVector<double> a_quad_em;
	QVector<int>    quad_elements;
	QVector<double> a_river_xyuv;

	QOpenGLShaderProgram m_shaderProgramRiver;
	QOpenGLShaderProgram m_shaderProgramLand;
	QOpenGLShaderProgram m_shaderProgramDepth;
	QOpenGLShaderProgram m_shaderProgramDrape;
	QOpenGLShaderProgram m_shaderProgramFinal;

	unsigned int m_frameBuffer;
	unsigned int buffer_quad_xy;
	unsigned int buffer_quad_em;
	unsigned int buffer_quad_elements;
	unsigned int buffer_river_xyuv;
};