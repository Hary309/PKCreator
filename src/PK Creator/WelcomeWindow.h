#pragma once

#include <ui_WelcomeWindow.h>

#include <QDialog>
#include <QList>

class QWidget;

class MainWindow;

struct Project
{
	QString name;
	qint64 timestamp;
	QString path;

	QTreeWidgetItem *item;
};

class WelcomeWindow : public QDialog
{
	Q_OBJECT

private:
	Ui::WelcomeWindow	m_ui;
	QList<Project*>		m_projectList;

	MainWindow			*m_pMainWnd;

public:
	WelcomeWindow(QWidget *parent = 0);
	~WelcomeWindow();

	void SaveList();
	void LoadList();

private slots:
	void CreateButton_clicked();
	void OpenButton_clicked();
	void DeleteButton_clicked();
	void OpenFolderButton_clicked();
};

