#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define QT_NO_KEYWORDS

#include <QMainWindow>
#include "cepy.hpp"
#include <python.h>
#include <QKeyEvent>
#include "GLWindow.h"
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(CePy *cePy, GLWidget *glWidget);


private Q_SLOTS:
	void onAddNew(CePy *cePy, GLWidget *glWidget);

protected:
	virtual void mousePressEvent(QMouseEvent *e);
	virtual void mouseMoveEvent(QMouseEvent *e);
	virtual void mouseReleaseEvent(QMouseEvent *e);

private:
	bool _mousePressed;
	QPoint _mousePosition;
	
};

#endif