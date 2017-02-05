
#define QT_NO_KEYWORDS
#include "GLWindow.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QtMath>
#include "modeller.hpp"
#include "shader.hpp"
#include <QTimer>
#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLPaintDevice>
#include <QPainter>



const int maxModelCount = 100;
int modelCount = 0;


Modeller scene[maxModelCount];
QOpenGLVertexArrayObject s_vao[maxModelCount];
QOpenGLVertexArrayObject screenQuad_vao;
QOpenGLBuffer s_vbo[maxModelCount];

GLWidget::GLWidget(QWidget *parent)
	: QOpenGLWidget(parent),
	m_xRot(0),
	m_yRot(0),
	m_zRot(0),
	shaded_shader(0),
	edges_shader(0),
	vertices_shader(0),
	screen_shader(0),
	tex(0)
{
	m_core = QCoreApplication::arguments().contains(QStringLiteral("--coreprofile"));
	m_transparent = QCoreApplication::arguments().contains(QStringLiteral("--transparent"));
	if (m_transparent) {
		QSurfaceFormat fmt = format();
		fmt.setAlphaBufferSize(8);
		setFormat(fmt);
	}

	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(0);

}

GLWidget::~GLWidget()
{
	cleanup();
}

static void qNormalizeAngle(int &angle)
{
	while (angle < 0)
		angle += 360 * 16;
	while (angle > 360 * 16)
		angle -= 360 * 16;
}

void GLWidget::setXRotation(int angle)
{
	qNormalizeAngle(angle);
	if (angle != m_xRot) {
		m_xRot = angle;
		Q_EMIT xRotationChanged(angle);
		update();
	}
}

void GLWidget::setYRotation(int angle)
{
	qNormalizeAngle(angle);
	if (angle != m_yRot) {
		m_yRot = angle;
		Q_EMIT yRotationChanged(angle);
		update();
	}
}

void GLWidget::setZRotation(int angle)
{
	qNormalizeAngle(angle);
	if (angle != m_zRot) {
		m_zRot = angle;
		Q_EMIT zRotationChanged(angle);
		update();
	}
}

void GLWidget::setLineWidth(int width)
{
	//printf("%i", width);
	lineWidth = float(width);
	Q_EMIT lineWidthChanged(width);
	update();
}
void GLWidget::setVertSize(int size)
{
	//printf("%i", size);
	vertSize = float(size);
	Q_EMIT vertSizeChanged(size);
	update();
}

QSize GLWidget::minimumSizeHint() const
{
	return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
	return QSize(400, 400);
}

void GLWidget::cleanup()
{
	makeCurrent();

	for (int i = 0; i < maxModelCount; i++)
	{
		scene[i].vbo().destroy();
	}

	delete shaded_shader;
	delete edges_shader;
	delete vertices_shader;
	delete screen_shader;
	shaded_shader = 0;
	edges_shader = 0;
	vertices_shader = 0;
	screen_shader = 0;
	doneCurrent();
}


static const char *vertexShaderSource =
"attribute vec4 vertex;\n"
"attribute vec3 normal;\n"
"attribute vec2 vertexUV;\n"
"varying vec3 vert;\n"
"varying vec3 vertNormal;\n"
"varying vec2 UV;\n"
"uniform mat4 projMatrix;\n"
"uniform mat4 mvMatrix;\n"
"uniform mat3 normalMatrix;\n"
"void main() {\n"
"   vert = vertex.xyz;\n"
"   vertNormal = normalMatrix * normal;\n"
"   gl_Position = projMatrix * mvMatrix * vertex;\n"
"   UV = vertexUV;\n"
"}\n";

static const char *fragmentEdges =
"varying highp vec3 vert;\n"
"varying highp vec3 vertNormal;\n"
"uniform highp vec3 lightPos;\n"
"void main() {\n"
"   highp vec3 L = normalize(lightPos - vert);\n"
"   highp float NL = max(dot(normalize(vertNormal), L), 0.0);\n"
"   highp vec3 color = vec3(0.39, 1.0, 0.0);\n"
"   highp vec3 col = clamp(color * 0.2 + color * 0.8 * NL, 0.0, 1.0);\n"
"   gl_FragColor = vec4(vert, 1.0);\n"
"}\n";

static const char *fragmentVertices =
"varying highp vec3 vert;\n"
"varying highp vec3 vertNormal;\n"
"uniform highp vec3 lightPos;\n"
"void main() {\n"
"   highp vec3 L = normalize(lightPos - vert);\n"
"   highp float NL = max(dot(normalize(vertNormal), L), 0.0);\n"
"   highp vec3 color = vec3(0.39, 1.0, 0.0);\n"
"   highp vec3 col = clamp(color * 0.2 + color * 0.8 * NL, 0.0, 1.0);\n"
"   gl_FragColor = vec4(vert, 1.0);\n"
"}\n";

static const char *fragmentShaded =
"varying highp vec3 vert;\n"
"varying highp vec3 vertNormal;\n"
"varying highp vec2 UV;\n"
"uniform sampler2D diffuseTexture;"
"uniform highp vec3 lightPos;\n"
"void main() {\n"
"   highp vec3 L = normalize(lightPos - vert);\n"
"   highp float NL = max(dot(normalize(vertNormal), L), 0.0);\n"
"   highp vec3 color = vec3(0.5, 0.5, 0.5);\n"
"   highp vec3 col = clamp(color * 0.01 + color * 0.8 * NL, 0.0, 1.0);\n"
"   gl_FragColor = vec4(UV,0, 1.0);\n"
//"   gl_FragColor = texture(diffuseTexture, UV );\n"
"}\n";


static const char *screenVertexShader =
"attribute vec4 vertex;\n"
"attribute vec3 normal;\n"
"attribute vec2 vertexUV;\n"
"varying vec3 vert;\n"
"varying vec3 vertNormal;\n"
"varying vec2 UV;\n"
"uniform mat4 projMatrix;\n"
"uniform mat4 mvMatrix;\n"
"uniform mat3 normalMatrix;\n"
"void main() {\n"
"   vert = vertex.xyz;\n"
"   vertNormal = normalMatrix * normal;\n"
"   gl_Position = projMatrix * mvMatrix * vertex;\n"
"   UV = vertexUV;\n"
"}\n";
static const char *screenFragmentShader =
"varying highp vec3 vert;\n"
"varying highp vec3 vertNormal;\n"
"varying highp vec2 UV;\n"
"uniform sampler2D diffuseTexture;"
"uniform highp vec3 lightPos;\n"
"void main() {\n"
"   highp vec3 L = normalize(lightPos - vert);\n"
"   highp float NL = max(dot(normalize(vertNormal), L), 0.0);\n"
"   highp vec3 color = vec3(0.5, 0.5, 0.5);\n"
"   highp vec3 col = clamp(color * 0.01 + color * 0.8 * NL, 0.0, 1.0);\n"
"   gl_FragColor = vec4(UV,0, 1.0);\n"
//"   gl_FragColor =  vec4(0.7,0.7,0.7,1.0)*texture(diffuseTexture, UV );\n"
"}\n";

void GLWidget::addModel(Modeller scene[], Modeller newModel)
{
	scene[modelCount] = newModel;
	modelCount += 1;
}

void GLWidget::setupScene()
{
	Modeller model;
	Modeller coolModel;
	//Add Some Geometry to models



	float w = 0.2f;
	float h = 0.2f;
	coolModel.quad(-w, h, -w, -h, w, -h, w, h);
	//model.quad(0.0f, -h, 0.0f, h, w, -h, w, 0.0f);
	model.quad(0.0f, -0.40f, 0.00f, -0.25f, 0.26f, -0.24f, 0.14f, -0.35f);

	//Add Model to Scene
	addModel(scene, coolModel);
	addModel(scene, model);
	//addModel(scene, coolModel);

	//Setup Each Model in Scene
	for (int i = 0; i < maxModelCount; i++)
	{
		setupModel(scene[i], &s_vao[i]);
	}
}

void GLWidget::initShader(	Shader *shader,
							static const char *vertexShaderSource, 
							static const char *fragmentShaderSource
)
{
	//Inits a shader program here
	shader->program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
	shader->program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
	shader->program->bindAttributeLocation("vertex", 0);
	shader->program->bindAttributeLocation("normal", 1);
	shader->program->bindAttributeLocation("vertexUV", 2);
	shader->program->link();
	shader->program->bind();
	shader->projMatrixLoc = shader->program->uniformLocation("projMatrix");
	shader->mvMatrixLoc = shader->program->uniformLocation("mvMatrix");
	shader->normalMatrixLoc = shader->program->uniformLocation("normalMatrix");
	shader->lightPosLoc = shader->program->uniformLocation("lightPos");
	shader->program->setUniformValue(shader->lightPosLoc, QVector3D(0, 0, 0.25f));
	shader->program->release();
}

void GLWidget::initializeGL()
{


	connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanup);
	initializeOpenGLFunctions();

	//GLint viewport[4];
	//glGetIntegerv(GL_VIEWPORT, viewport);

	setupScene();
	ortho_camera.setToIdentity();
	m_camera.setToIdentity();
	m_camera.translate(0, 0, -1);
	//glDisable(GL_CULL_FACE);
	glClearColor(0.1f, 0.1f, 0.1f, 1);

	shaded = new Shader;
	lines = new Shader;
	points = new Shader;
	
	initShader(shaded, vertexShaderSource, fragmentShaded);
	initShader(lines, vertexShaderSource, fragmentEdges);
	initShader(points, vertexShaderSource, fragmentVertices);
	
	//fboFormat.setAttachment(QOpenGLFramebufferObject::Depth);


	screen = new Shader;
	initShader(screen, screenVertexShader, screenFragmentShader);

	//Screen Quad
	float size = 1.0f;
	screenQuad.quad(-1.0f*size,-1.0f*size,
					1.0f*size, -1.0f*size,
					1.0f*size, 1.0f*size,
					-1.0f*size, 1.0f*size);
	setupModel(screenQuad, &screenQuad_vao);

}
void GLWidget::setupModel(Modeller modeller, QOpenGLVertexArrayObject * vao)
{
	vao->create();
	QOpenGLVertexArrayObject::Binder vaoBinder(vao);

	modeller.vbo().create();
	modeller.vbo().bind();
	modeller.vbo().allocate(modeller.constData(), modeller.count() * sizeof(GLfloat));
	setupVertexAttribs(modeller.vbo());
}

void GLWidget::updateModel(Modeller modeller)
{
	modeller.vbo().bind();
	modeller.vbo().allocate(modeller.constData(), modeller.count() * sizeof(GLfloat));
}

void GLWidget::setupVertexAttribs(QOpenGLBuffer vbo)
{
	vbo.bind();
	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	f->glEnableVertexAttribArray(0);
	f->glEnableVertexAttribArray(1);
	f->glEnableVertexAttribArray(2);

	f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
	f->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<void *>(6 * sizeof(GLfloat)));
	vbo.release();
}

void GLWidget::renderModel(Modeller model, 
						   Shader *shader, 
						   QOpenGLVertexArrayObject * vao,
						   QMatrix4x4 camera, QMatrix4x4 projection, QMatrix4x4 world,
						   BOOLEAN renderVertices, BOOLEAN renderEdges, BOOLEAN renderShaded)
{
	QOpenGLVertexArrayObject::Binder vaoBinder(vao);
	shader->program->bind();
	shader->program->setUniformValue(shader->projMatrixLoc, projection);
	shader->program->setUniformValue(shader->mvMatrixLoc, camera * world);
	QMatrix3x3 normalMatrix = world.normalMatrix();
	shader->program->setUniformValue(shader->normalMatrixLoc, normalMatrix);

	if (renderShaded == TRUE)
	{
		glDisable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);
		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_TRIANGLES, 0, model.vertexCount());
	}
	if (renderEdges == TRUE)
	{
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(lineWidth);
		glDrawArrays(GL_TRIANGLES, 0, model.vertexCount());
	}
	if (renderVertices == TRUE)
	{
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glPointSize(vertSize);
		glDrawArrays(GL_TRIANGLES, 0, model.vertexCount());
	}

	shader->program->release();
}

Modeller GLWidget::translate(Modeller model, QVector3D vector3)
{
	for (int i = 0; i < model.vertexCount(); i++)
	{
		int vertex = i + 1;
		int x = 3;
		int y = 2;
		int z = 1;
		int index = (8 * vertex) - (x + 5);
		model.data[index] = model.data[index] + vector3.x();
		index = (8 * vertex) - (y + 5);
		model.data[index] = model.data[index] + vector3.y();
		index = (8 * vertex) - (z + 5);
		model.data[index] = model.data[index] + vector3.z();
	}
	return model;
}

void GLWidget::paintGL()
{
	

	
	time += 0.01f;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	
	
	m_world.setToIdentity();
	m_world.rotate(180.0f - (m_xRot / 16.0f), 1, 0, 0);
	m_world.rotate(m_yRot / 16.0f, 0, 1, 0);
	m_world.rotate(m_zRot / 16.0f, 0, 0, 1);
	
	float sinTime = (qSin(time*2.0f));
	float sinTime2 = (qSin((time+0.5)*10.0f));
	//printf("time: %f \n", sinTime);

	scene[0] = translate(scene[0], QVector3D(sinTime*0.0005f, 0.0f, 0.0f));
	scene[0] = translate(scene[0], QVector3D(0.0f, sinTime2*0.001f, 0.0f));
	
	for (int i = 0; i < maxModelCount; i++)
	{
		updateModel(scene[i]);
	}

	
	
	for (int i = 0; i < maxModelCount; i++)
	{
		renderModel(scene[i], shaded, &s_vao[i], m_camera, m_proj, m_world, FALSE, FALSE, TRUE);
		renderModel(scene[i], points, &s_vao[i], m_camera, m_proj, m_world, FALSE, TRUE, FALSE);
		renderModel(scene[i], lines, &s_vao[i], m_camera, m_proj, m_world, TRUE, FALSE, FALSE);
		
	}
	
	
	//fbo = new QOpenGLFramebufferObject(QSize(width, height), fboFormat);
	//fbo.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
	//fbo->bind();
	

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//tex = new QOpenGLTexture(fbo->toImage().mirrored());
	//tex = new QOpenGLTexture(QImage(QString("image.png")).mirrored());
	//tex->bind();
	//fbo->release();



	//ortho_world.setToIdentity();
	//ortho_proj.setToIdentity();
	//ortho_proj.ortho(QRect(0.0, 0.0, -1.0, 1.0));
	//ortho_proj.ortho(QRect(-0.5, -0.5, 0.5, 0.5));
	//ortho_proj.ortho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	//renderModel(screenQuad, screen, &screenQuad_vao, ortho_camera, ortho_proj, ortho_world, FALSE, FALSE, TRUE);
	//renderModel(screenQuad, points, &screenQuad_vao, ortho_camera, ortho_proj, ortho_world, TRUE, FALSE, FALSE);
	//renderModel(screenQuad, lines, &screenQuad_vao, ortho_camera, ortho_proj, ortho_world, FALSE, TRUE, FALSE);
	

	//glViewport(0, 0, 1024, 768);
}

void GLWidget::resizeGL(int w, int h)
{
	width = w;
	height = h;
	m_proj.setToIdentity();
	m_proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);

	ortho_proj.setToIdentity();
	ortho_proj.ortho(QRect(0, 0, width, height));
	
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	//mouse_last_pos = QPointF(float(event->pos().x()) / float(width), float(event->pos().y()) / float(height));
	m_lastPos = event->pos();

	if (event->buttons() & Qt::LeftButton) {
		//printf("Image saved");
		//fbo->toImage().save("image.png");
		//printf("%f,%f \n", mouse_last_pos.x(), mouse_last_pos.y());
	}
	else if (event->buttons() & Qt::RightButton) {

	}
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	//mouse_pos = QPointF(float(event->pos().x()) / float(width), float(event->pos().y()) / float(height));


	int dx = event->x() - m_lastPos.x();
	int dy = event->y() - m_lastPos.y();

	if (event->buttons() & Qt::LeftButton) {
		setXRotation(m_xRot + 8 * dy);
		setYRotation(m_yRot + 8 * dx);
	}
	else if (event->buttons() & Qt::RightButton) {
		//setXRotation(m_xRot + 8 * dy);
		//setZRotation(m_zRot + 8 * dx);
		m_camera.translate(0, 0, (float(1 - dy) / float(height))*1.5f);
	}
	else if (event->buttons() & Qt::MiddleButton) {
		m_camera.translate((float(dx)/float(width))*0.5f,( float(1-dy)/float(height))*0.5f, 0);
	}

	m_lastPos = event->pos();

}

void GLWidget::mouseReleaseEvent(QMouseEvent * event)
{
	//QPoint mouse_at_release = event->pos();

	// example code
	//QPoint dx = mouse_at_release - mouse_last_pos;

	if (event->buttons() & Qt::LeftButton) {

	}
	else if (event->buttons() & Qt::RightButton) {

	}
}