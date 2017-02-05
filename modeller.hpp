#ifndef MODELLER_H
#define MODELLER_H
#define QT_NO_KEYWORDS
#include <qopengl.h>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QCoreApplication>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

class Modeller
{
public:
	Modeller();
	const GLfloat *constData() const { return data.constData(); }
	int count() const { return m_count; }
	int vertexCount() const { return m_count / 8; }
	QOpenGLBuffer vbo() { return vertex_buffer_object; };
	

	void quad(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, GLfloat x4, GLfloat y4);
	void extrude(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
	void add(const QVector3D &v, const QVector3D &n, const QVector2D &uv);

	QVector<GLfloat> data;
	
	QOpenGLBuffer vertex_buffer_object;
	int m_count;

	


};

#endif // MODELLER_H
