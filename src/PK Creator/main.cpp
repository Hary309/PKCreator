#include "WelcomeWindow.h"
#include <QtWidgets/QApplication>

#include <Windows.h>

int main(int argc, char **argv)
{
	AllocConsole();
	freopen("CONOUT$", "w", stdout);

	QApplication app(argc, argv);
	WelcomeWindow window;
	window.show();
	return app.exec();
}
