#include "MainWindow.h"
#include <QtWidgets/QApplication>

#include <Windows.h>

int main(int argc, char *argv[])
{
	AllocConsole();
	freopen("CONOUT$", "w", stdout);

	QApplication app(argc, argv);
	MainWindow window;
	window.show();
	return app.exec();
}
