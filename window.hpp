#ifndef WINDOW_H
#define WINDOW_H
#define QT_NO_KEYWORDS

#include <QWidget>
#include <QTextEdit>
#include "cepy.hpp"
#include <python.h>
#include "scripteditor.hpp"

QT_BEGIN_NAMESPACE
class QSlider;
class QPushButton;
QT_END_NAMESPACE

class GLWidget;
class MainWindow;

class Window : public QWidget
{
	Q_OBJECT

public:
	Window(MainWindow *mw, CePy *cePy, GLWidget * glWidget);
	void executePythonScript();
	CePy *pythonShell;
	//GLWidget *glWidget;


private Q_SLOTS:
	QSlider *createSlider();
	
	
Q_SIGNALS:
	void *enterKeyPressed();

private:
	MainWindow *mainWindow;
	QSlider *lineSlider;
	QSlider *vertSlider;
	ScriptEditor *scriptEditor;
};

#endif