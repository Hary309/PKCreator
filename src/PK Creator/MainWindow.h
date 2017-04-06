#pragma once

#include "ui_MainWindow.h"

class ResourceView;
class ObjectWindow;
class Project;

class QStandardItem;

class MainWindow : public QMainWindow
{
	Q_OBJECT

private:
	Ui::MainWindowClass				m_ui;
	ResourceView					*m_pResView;

	Project							*m_proInfo;

	static MainWindow				*s_pInst;

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	bool Load(Project *project);
	void Save() const;

	static MainWindow *Get() { return s_pInst; }

private slots:
	void ActionExit_triggered();
	void ActionSaveProject_triggered() const;
	void ActionOpenProject_triggered();
};
