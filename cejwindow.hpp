#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define QT_NO_KEYWORDS

#include <QMainWindow>
#include "cepy.hpp"
#include <python.h>
#include <QDesktopWidget>
#include <QApplication>
#include <QKeyEvent>
#include "GLWindow.h"
#include <QWidget>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(CePy *cePy, GLWidget *glWidget, QApplication *app);

	QApplication *mainApp;

protected:
	void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

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