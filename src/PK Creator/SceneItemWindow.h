/*
*********************************************************************
* File          : SceneItemWindow.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include "ui_SceneWindow.h"

class SceneItem;
class SceneEditor;
class Item;

#include <ItemWindow.h>

#include <QVector>
#include <QSharedPointer>

class SceneItemWindow : public ItemWindow
{
	Q_OBJECT

	struct ObjectListItem
	{
		QListWidgetItem		*listItem;
		long long			id;
	};

private:
	Ui::SceneWindow								m_ui;
	SceneItem									*m_pItemParent;
	QSharedPointer<SceneEditor>					m_pEditor;

	QVector<QSharedPointer<ObjectListItem>>		m_objectsList;

protected:
	void enterEvent(QEvent *event) override;
	void closeEvent(QCloseEvent *event) override;
	void showEvent(QShowEvent *event) override;

public:
	explicit SceneItemWindow(QWidget *parent);
	virtual ~SceneItemWindow();

	void RefreshObjectList();

	ObjectListItem *GetObjectListItem(QListWidgetItem *item);

	bool FillData(Item *item) override;

private slots:
	void OkButton_clicked();
	void BgColorButton_clicked();
	void ObjectList_ItemClicked(QListWidgetItem *item);

	void snapXBox_valueChanged(int i);
	void snapYBox_valueChanged(int i);

	void DrawGrid_stateChanged(int state);

};

