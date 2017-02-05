#define QT_NO_KEYWORDS

#include "GLWindow.h"
#include "window.hpp"
#include "cejwindow.hpp"
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QPushButton>
#include <QDesktopWidget>
#include <QApplication>
#include <QMessageBox>
#include <QSplitter>
#include <QSlider>
#include <QTextEdit>
#include "cepy.hpp"
#include <python.h>
#include "scripteditor.hpp"

Window::Window(MainWindow *mw, CePy *cePy, GLWidget * glWidget)
	: mainWindow(mw)
{
	pythonShell = cePy;
	
	
	lineSlider = createSlider();
	vertSlider = createSlider();
	//lineSlider->setValue(0);
	//vertSlider->setValue(10);
	pythonShell->glWidget = glWidget;
	
	
	connect(lineSlider, &QSlider::valueChanged, glWidget, &GLWidget::setLineWidth);
	connect(glWidget, &GLWidget::lineWidthChanged, lineSlider, &QSlider::setValue);
	connect(vertSlider, &QSlider::valueChanged, glWidget, &GLWidget::setVertSize);
	connect(glWidget, &GLWidget::vertSizeChanged, vertSlider, &QSlider::setValue);
	

	QVBoxLayout *mainLayout = new QVBoxLayout;
	QHBoxLayout *container = new QHBoxLayout;
	QSplitter *splitter = new QSplitter;
	container->addWidget(lineSlider);
	container->addWidget(vertSlider);
	container->setSizeConstraint(QLayout::SetFixedSize);

	QWidget *w = new QWidget;
	w->setLayout(container);

	scriptEditor = new ScriptEditor;
	scriptEditor->setTabStopWidth(15);
	QPalette pal;
	//pal.setColor(QPalette::Window, bgColor);
	//pal.setColor(QPalette::WindowText, fgColor);
	pal.setColor(QPalette::Text, QColor(255,255,255));
	scriptEditor->setPalette(pal);

	mainLayout->addWidget(w);
	mainLayout->addWidget(glWidget);
	mainLayout->addWidget(scriptEditor);
	setLayout(mainLayout);	
	
	//connect(scriptEditor, &ScriptEditor::execute, this, &Window::executePythonScript);
	QObject::connect(scriptEditor, &ScriptEditor::execute, this, &Window::executePythonScript);
	
}

QSlider *Window::createSlider()
{
	QSlider *slider = new QSlider(Qt::Horizontal);
	slider->setRange(0, 50);
	slider->setSingleStep(16);
	slider->setPageStep(15 * 16);
	slider->setTickInterval(15 * 16);
	slider->setTickPosition(QSlider::TicksRight);
	return slider;
}

void Window::executePythonScript()
{
	
	char *pyArgs[2] = { "8","2" };
	//pythonShell->runModule("pythonCode.py", "multiply", pyArgs);
	QString Qscript = scriptEditor->toPlainText();
	QByteArray ba = Qscript.toLocal8Bit();
	const char *script = ba.data();

	pythonShell->execScript(script);
	//qDebug() << scriptEditor->toPlainText();
}

void Window::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_Escape)
	{ 
		close();
	}
	else if (e->key() == Qt::Key_Enter)
	{ 
		//printf("Enter Pressed\n");
		//Q_EMIT this->enterKeyPressed();
	}
	else 
	{
		QWidget::keyPressEvent(e);

	}
}

