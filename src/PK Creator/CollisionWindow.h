/*
*********************************************************************
* File          : CollisionWindow.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include "ui_CollisionWindow.h"


class Item;

class CollisionWindow : public QDialog
{
	Q_OBJECT

	class WidgetItem
	{
	public:
		QListWidgetItem *listWidgetItem;
		Item			*object;
	};

private:
	Ui::CollisionWindow						m_ui;

	QVector<QSharedPointer<WidgetItem>>		m_objectList;

	Item									*m_selectedObject;

public:
	CollisionWindow(QWidget *parent);
	~CollisionWindow() = default;

	void exec(Item *item);

	Item *GetSelectedItem() const { return m_selectedObject; }

private slots:
	void SelectButton_clicked();
	void CancelButton_clicked();
};

