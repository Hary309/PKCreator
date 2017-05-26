/*
*********************************************************************
* File          : MainWindow.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

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

	void GenerateCode(bool showInformation);

	static MainWindow *Get() { return s_pInst; }

private slots:
	void ActionOpenProject_triggered();
	void ActionOpenContainingFolder_triggered();
	void ActionConfig_triggered();
	void ActionRun_triggered();
};
