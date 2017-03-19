#pragma once

#include "ui_SceneWindow.h"

class SceneItem;
class SceneEditor;
class Item;

#include <ItemWindow.h>

#include <QList>

class SceneItemWindow : public ItemWindow
{
	Q_OBJECT

	struct ObjectListItem
	{
		QListWidgetItem		*listItem;
		long long			id;
	};

private:
	Ui::SceneWindow			ui;
	SceneItem				*m_pItemParent;
	SceneEditor				*m_pEditor;

	QList<ObjectListItem*>  m_objectsList;

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

