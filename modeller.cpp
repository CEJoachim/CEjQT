#define QT_NO_KEYWORDS
#include "modeller.hpp"

#include <qmath.h>

Modeller::Modeller()
	: m_count(0)
{
	data.resize(2500 * 6);

}

void Modeller::add(const QVector3D &v, const QVector3D &n, const QVector2D &uv)
{

	GLfloat *p = data.data() + m_count;
	//Verts
	*p++ = v.x();
	*p++ = v.y();
	*p++ = v.z();
	//Normals
	*p++ = n.x();
	*p++ = n.y();
	*p++ = n.z();
	//UVs
	*p++ = uv.x();
	*p++ = uv.y();
	m_count += 8;
}

void Modeller::quad(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, GLfloat x4, GLfloat y4)
{
	/*
	QVector3D n = QVector3D::normal(QVector3D(x4 - x1, y4 - y1, 0.0f), QVector3D(x2 - x1, y2 - y1, 0.0f));
	
	add(QVector3D(x1, y1, -0.05f), n);
	add(QVector3D(x4, y4, -0.05f), n);
	add(QVector3D(x2, y2, -0.05f), n);

	add(QVector3D(x3, y3, -0.05f), n);
	add(QVector3D(x2, y2, -0.05f), n);
	add(QVector3D(x4, y4, -0.05f), n);*/
	
	QVector3D n = QVector3D::normal(QVector3D(x1 - x4, y1 - y4, 0.0f), QVector3D(x2 - x4, y2 - y4, 0.0f));

	add(QVector3D(x4, y4, 0.05f), n, QVector2D(0.0f, 1.0f));
	add(QVector3D(x1, y1, 0.05f), n, QVector2D(0.0f, 0.0f));
	add(QVector3D(x2, y2, 0.05f), n, QVector2D(1.0f, 0.0f));

	add(QVector3D(x2, y2, 0.05f), n, QVector2D(1.0f, 0.0f));
	add(QVector3D(x3, y3, 0.05f), n, QVector2D(1.0f, 1.0f));
	add(QVector3D(x4, y4, 0.05f), n, QVector2D(0.0f, 1.0f));
}

void Modeller::extrude(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
	QVector3D n = QVector3D::normal(QVector3D(0.0f, 0.0f, -0.1f), QVector3D(x2 - x1, y2 - y1, 0.0f));

	add(QVector3D(x1, y1, +0.05f), n, QVector2D(0.0f, 0.0f));
	add(QVector3D(x1, y1, -0.05f), n, QVector2D(1.0f, 1.0f));
	add(QVector3D(x2, y2, +0.05f), n, QVector2D(0.0f, 1.0f));

	add(QVector3D(x2, y2, -0.05f), n, QVector2D(0.0f, 0.0f));
	add(QVector3D(x2, y2, +0.05f), n, QVector2D(1.0f, 0.0f));
	add(QVector3D(x1, y1, -0.05f), n, QVector2D(1.0f, 1.0f));
}
