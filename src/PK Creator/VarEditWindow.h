/*
*********************************************************************
* File          : VarEditWindow.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <QDialog>
#include "ui_VarEditWindow.h"

#include <VariablesWindow.h>

class VarEditWindow : public QDialog
{
	Q_OBJECT

private:
	Ui::VarEditWindow m_ui;

	bool m_accepted;

public:
	VarEditWindow(QWidget *parent = Q_NULLPTR);
	~VarEditWindow();

	void SetVar(VariablesWindow::Var *var);
	VariablesWindow::Var *GetVar();

private slots:
	void OkButton_clicked();
	void TypeComboBox_currentIndexChanged(int index);
};
