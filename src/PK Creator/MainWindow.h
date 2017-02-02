#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

class ResourceView;
class ObjectWindow;

class QStandardItem;

class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	Ui::MainWindowClass		ui;
	ResourceView			*m_pResView;

	static MainWindow		*s_pInst;

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

	static MainWindow *Get() { return s_pInst; }

private slots:
	void ActionExit_clicked();
};
