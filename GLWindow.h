#ifndef GLWIDGET_H
#define GLWIDGET_H
#define QT_NO_KEYWORDS
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include "Modeller.hpp"
#include "shader.hpp"
#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLTexture>
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)




class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	GLWidget(QWidget *parent = 0);
	~GLWidget();

	QSize minimumSizeHint() const Q_DECL_OVERRIDE;
	QSize sizeHint() const Q_DECL_OVERRIDE;

public Q_SLOTS:
	void cleanup();
	void setXRotation(int angle);
	void setYRotation(int angle);
	void setZRotation(int angle);
	void setLineWidth(int width);
	void setVertSize(int size);

Q_SIGNALS:
	void xRotationChanged(int angle);
	void yRotationChanged(int angle);
	void zRotationChanged(int angle);
	void lineWidthChanged(int width);
	void vertSizeChanged(int size);


protected:
	void initializeGL() Q_DECL_OVERRIDE;
	void paintGL() Q_DECL_OVERRIDE;
	void resizeGL(int width, int height) Q_DECL_OVERRIDE;
	void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
	

private:
	void setupVertexAttribs(QOpenGLBuffer vbo);
	void setupModel(Modeller modeller, QOpenGLVertexArrayObject * vao);
	void renderModel(Modeller model, Shader *shader, QOpenGLVertexArrayObject * vao, 
					QMatrix4x4 camera, QMatrix4x4 projection, QMatrix4x4 world,
					BOOLEAN renderVertices, BOOLEAN renderEdges, BOOLEAN renderShaded);
	void setupScene();
	void updateModel(Modeller modeller);
	void addModel(Modeller scene[], Modeller newModel);
	Modeller translate(Modeller model, QVector3D vector3);
	void GLWidget::initShader(	Shader *shader,
								static const char *vertexShaderSource, 
								static const char *fragmentShaderSource
	);

	int m_xRot;
	int m_yRot;
	int m_zRot;

	float lineWidth = 5.0f;
	float vertSize = 10.0f;

	bool m_core;
	QPointF mouse_last_pos;
	QPointF mouse_pos;
	QPoint m_lastPos;

	int width;
	int height;
	Modeller screenQuad;
	float time = 0.0f;

	QOpenGLFramebufferObjectFormat fboFormat;

	QOpenGLShaderProgram *shaded_shader;
	QOpenGLShaderProgram *edges_shader;
	QOpenGLShaderProgram *vertices_shader;
	QOpenGLShaderProgram *screen_shader;
	QOpenGLTexture * tex;
	QOpenGLFramebufferObject *fbo;
	Shader *shaded;
	Shader *lines;
	Shader *points;
	Shader *screen;

	int m_projMatrixLoc;
	int m_mvMatrixLoc;
	int m_normalMatrixLoc;
	int m_lightPosLoc;
	QMatrix4x4 m_proj;
	QMatrix4x4 m_camera;
	QMatrix4x4 m_world;

	QMatrix4x4 ortho_proj;
	QMatrix4x4 ortho_camera;
	QMatrix4x4 ortho_world;


	int m_projMatrixLoc2;
	int m_mvMatrixLoc2;
	int m_normalMatrixLoc2;
	

	bool m_transparent;
};

#endif
