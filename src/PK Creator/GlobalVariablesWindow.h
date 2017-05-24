/*
*********************************************************************
* File          : GlobalVariablesWindow.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include "ui_VariablesWindow.h"

class Var;

class HTML5Generator;

class GlobalVariablesWindow : public QDialog
{
	Q_OBJECT

private:
	Ui::VariablesWindow m_ui;

	QVector<QSharedPointer<Var>> m_vars;

	static GlobalVariablesWindow *s_pInst;

	friend HTML5Generator;

private:
	void AddVar(Var *var);
	void AddVarToVisualList(Var *var) const;

public:
	explicit GlobalVariablesWindow(QWidget *parent = Q_NULLPTR);
	~GlobalVariablesWindow() = default;

	Var *GetVar(qint64 id);

	auto GetAllVars() const { return &m_vars; }

	void Load(QDataStream *const dataStream);
	void Save(QDataStream *const dataStream);

	static GlobalVariablesWindow *Get() { return s_pInst; }

private:
	void AddVarButton_clicked();
	void EditVarButton_clicked();
	void RemoveVarButton_clicked();
};

