#ifndef CEPY_H
#define CEPY_H

#include <QWidget>
#include <QSlider>
#include <Python.h>
#include "GLWindow.h"

class CePy
{
public:
	CePy(int argc, char *argv[]);
	~CePy();

	wchar_t *program;
	PyObject *pName;
	PyObject *pModule;
	PyObject *pDict;
	PyObject *pFunc;
	PyObject *pArgs;
	PyObject *pValue;
	int runModule(char *module, char *function, char *argv[]);
	int CePy::execScript(const char *script);
	

	static QSlider *lineSlider;
	QSlider *vertSlider;
	GLWidget *glWidget;

	static void CePy::pyCalls();

Q_SIGNALS:
	static void *pyCall();
};

#endif 
