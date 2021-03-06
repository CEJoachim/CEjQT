﻿#define QT_NO_KEYWORDS

#include "cejwindow.hpp"
#include "GLWindow.h"
#include "window.hpp"
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "cepy.hpp"
#include <python.h>
#include <QKeyEvent>
#include <QWidget>


MainWindow::MainWindow(CePy *cePy, GLWidget *glWidget, QApplication *app):
	_mousePressed(false),
	_mousePosition(QPoint())
{
	mainApp = app;
	//setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
	QMenuBar *menuBar = new QMenuBar;
	QMenu *menuWindow = menuBar->addMenu(tr("&File"));
	QMenu *menuEdit = menuBar->addMenu(tr("&Edit"));
	QMenu *menuView = menuBar->addMenu(tr("&View"));
	QMenu *menuProject = menuBar->addMenu(tr("&Project"));
	QMenu *menuHelp = menuBar->addMenu(tr("&Help"));
	QAction *addNew = new QAction(menuWindow);
	addNew->setText(tr("Add new"));
	menuWindow->addAction(addNew);
	setMenuBar(menuBar);
	setWindowTitle(tr("Albert"));
	onAddNew(cePy, glWidget);
	
}

void MainWindow::onAddNew(CePy *cePy, GLWidget *glWidget)
{
	if (!centralWidget())
		setCentralWidget(new Window(this, cePy, glWidget));
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton) {
		//_mousePressed = true;
		//_mousePosition = e->pos();
	}
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
	if (_mousePressed) {
		//move(mapToParent(e->pos() - _mousePosition));
	}
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton) {
		//_mousePressed = false;
		//_mousePosition = QPoint();
	}
}

void MainWindow::keyPressEvent(QKeyEvent *e)
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