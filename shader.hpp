#ifndef SHADER_H
#define SHADER_H
#define QT_NO_KEYWORDS

#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class Shader
{
public:
	Shader();

	QOpenGLShaderProgram *program;

	int projMatrixLoc;
	int mvMatrixLoc;
	int normalMatrixLoc;
	int lightPosLoc;
	//QMatrix4x4 proj;
	//QMatrix4x4 camera;
	//QMatrix4x4 world;

private:
	
};


#endif