/*
*********************************************************************
* File          : main.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "WelcomeWindow.h"

#include <QtWidgets/QApplication>
#include <QDir>
#include <QDateTime>
#include <QLoggingCategory>

QString g_logFileName;

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	QString text = "[" + QString(context.file) + ": " + QString::number(context.line) + "] <" + QDateTime::currentDateTime().toString("hh:mm:ss.zzz") + "> ";

	switch (type)
	{
	case QtInfoMsg:     text += "Info: "; break;
	case QtDebugMsg:    text += "Debug: "; break;
	case QtWarningMsg:  text += "Warning: "; break;
	case QtCriticalMsg: text += "Critical: "; break;
	case QtFatalMsg:    text += "Fatal: "; break;
	}

	text += msg;

	puts(text.toStdString().c_str());

	QFile outFile(g_logFileName);
	outFile.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream textStream(&outFile);
	textStream << text << endl;
}

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

	g_logFileName = "log_" + QDate::currentDate().toString("dd-MM-yyyy") + ".txt";

	QApplication app(argc, argv);
	qInstallMessageHandler(messageHandler);
	WelcomeWindow window;
	window.show();
	return app.exec();
}
