/*
*********************************************************************
* File          : VariablesWindow.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include "ui_VariablesWindow.h"

#include <QString>
#include <Common.h>

class Var;

class VariablesWindow : public QDialog
{
	Q_OBJECT

private:
	Ui::VariablesWindow m_ui;

	QVector<QSharedPointer<Var>> *m_pVars;

public:
	explicit VariablesWindow(QWidget *parent = Q_NULLPTR);
	~VariablesWindow();

	void SetSource(QVector<QSharedPointer<Var>> *vars);

	void AddVar(Var *var);

	void AddToVisualList(Var *var);

private slots:
	void AddVarButton_clicked();
	void EditVarButton_clicked();
	void RemoveVarButton_clicked();
};
