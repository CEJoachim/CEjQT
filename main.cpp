#define QT_NO_KEYWORDS
#include <QApplication>
#include <QDesktopWidget>
#include <QSurfaceFormat>
#include "GLWindow.h"
#include "cejwindow.hpp"

#include "cepy.hpp"
#include <python.h>

int main(int argc, char *argv[])
{
	
	CePy cePy(argc,argv);
	char *module = "pythonCode.py";
	char *function = "multiply";
	char *pyArgs[2] = { "3","15" };
	cePy.runModule("pythonCode.py", "multiply", pyArgs);
	

	

	QApplication app(argc, argv);
	app.setStyle("fusion");
	GLWidget glWidget = new GLWidget;
	
	//glWidget.setLineWidth(0);
	//glWidget.setVertSize(0);

	QSurfaceFormat fmt;
	fmt.setDepthBufferSize(24);
	if (QCoreApplication::arguments().contains(QStringLiteral("--multisample")))
		fmt.setSamples(4);
	if (QCoreApplication::arguments().contains(QStringLiteral("--coreprofile"))) {
		fmt.setVersion(3, 2);
		fmt.setProfile(QSurfaceFormat::CoreProfile);
	}
	QSurfaceFormat::setDefaultFormat(fmt);

	MainWindow mainWindow(&cePy, &glWidget, &app);
	mainWindow.setStyleSheet("background-color: rgb(50,50,50)");

	

	if (QCoreApplication::arguments().contains(QStringLiteral("--transparent"))) {
		mainWindow.setAttribute(Qt::WA_TranslucentBackground);
		mainWindow.setAttribute(Qt::WA_NoSystemBackground, false);
	}
	mainWindow.resize(mainWindow.sizeHint());
	int desktopArea = QApplication::desktop()->width() *
		QApplication::desktop()->height();
	int widgetArea = mainWindow.width() * mainWindow.height();
	if (((float)widgetArea / (float)desktopArea) < 0.75f)
	{ 
		mainWindow.show();
		glWidget.setLineWidth(0);
	}
	else
		mainWindow.showMaximized();
		


	return app.exec();
}