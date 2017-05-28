/*
*********************************************************************
* File          : WelcomeWindow.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <ui_WelcomeWindow.h>

#include <QDialog>
#include <QVector>

#include <QSharedPointer>

#include <Windows.h>

class QWidget;

class MainWindow;

struct Project
{
	QString name;
	QString path;
	qint64 timestamp;

	QTreeWidgetItem *item;
};

class WelcomeWindow : public QDialog
{
	Q_OBJECT

private:
	Ui::WelcomeWindow					m_ui;
	QVector<QSharedPointer<Project>>	m_projectList;

	QSharedPointer<MainWindow>			m_pMainWnd;

	static WelcomeWindow				*s_pInst;

public:
	explicit WelcomeWindow(QWidget *parent = nullptr);
	~WelcomeWindow();

	void SaveList();
	void LoadList();

	static WelcomeWindow *Get() { return s_pInst; }

private slots:
	void CreateButton_clicked();
	void OpenExistingProjectButton_clicked();
	void OpenButton_clicked();
	void DeleteButton_clicked();
	void OpenFolderButton_clicked();
};

