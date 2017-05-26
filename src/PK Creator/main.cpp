/*
*********************************************************************
* File          : main.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "WelcomeWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char **argv)
{
	for (int i = 0; i < argc; ++i)
	{
		if (strcmp(argv[i], "-show-console") == 0)
		{
			AllocConsole();
			freopen("CONOUT$", "w", stdout);
		}
	}
	
	QApplication app(argc, argv);
	WelcomeWindow window;
	window.show();
	return app.exec();
}
