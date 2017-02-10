#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

class ResourceView;
class ObjectWindow;
class Project;

class QStandardItem;

class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	Ui::MainWindowClass		ui;
	ResourceView			*m_pResView;

	Project					*m_proInfo;

	static MainWindow		*s_pInst;

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

	void Load(Project *project);
	void Save();

	static MainWindow *Get() { return s_pInst; }

private slots:
	void ActionExit_triggered();
	void ActionSaveProject_triggered();
	void ActionOpenProject_triggered();
};
