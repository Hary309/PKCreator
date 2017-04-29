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

class VariablesWindow : public QDialog
{
	Q_OBJECT

public:
	class Var
	{
	public:
		QString m_name;
		DataType m_type;

		QTreeWidgetItem *m_pItem;

		qint64 m_id; // auto generating

		union
		{
			int integer;
			float number;
			QString *string;
			bool boolean;
		} m_data;

		Var() { m_id = qint64(this); m_data.string = nullptr; m_pItem = nullptr; }
		~Var() { if (m_type == STRING) delete m_data.string; }
	};

private:
	Ui::VariablesWindow m_ui;

	QVector<QSharedPointer<Var>> *m_pVars;

public:
	explicit VariablesWindow(QWidget *parent = Q_NULLPTR);
	~VariablesWindow();

	void SetSource(void *vars);

	void AddVar(Var *var);

	void AddToVisualList(Var *var);

private slots:
	void AddVarButton_clicked();
	void EditVarButton_clicked();
	void RemoveVarButton_clicked();
};
