/*
*********************************************************************
* File          : VarEditWindow.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include "ui_VarEditWindow.h"

class Var;

class VarEditWindow : public QDialog
{
	Q_OBJECT

private:
	Ui::VarEditWindow m_ui;

	bool m_accepted;

public:
	explicit VarEditWindow(QWidget *parent = Q_NULLPTR);
	~VarEditWindow();

	void SetVar(Var *var);
	Var *GetVar();

private slots:
	void OkButton_clicked();
	void TypeComboBox_currentIndexChanged(int index);
};
