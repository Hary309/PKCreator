/*
*********************************************************************
* File          : LocalVariablesWindow.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include "ui_VariablesWindow.h"

class Var;

class LocalVariablesWindow : public QDialog
{
	Q_OBJECT

private:
	Ui::VariablesWindow m_ui;

	QVector<QSharedPointer<Var>> *m_pVars;

private:
	void AddVar(Var *var);
	void AddVarToVisualList(Var *var);

public:
	explicit LocalVariablesWindow(QWidget *parent = Q_NULLPTR);
	~LocalVariablesWindow() = default;

	void SetSource(QVector<QSharedPointer<Var>> *vars);

private slots:
	void AddVarButton_clicked();
	void EditVarButton_clicked();
	void RemoveVarButton_clicked();
};
