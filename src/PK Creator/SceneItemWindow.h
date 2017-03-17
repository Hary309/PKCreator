#pragma once

#include "ui_SceneWindow.h"

class SceneItem;
class SceneEditor;
class Item;

#include <ItemWindow.h>

class SceneItemWindow : public ItemWindow
{
	Q_OBJECT

private:
	Ui::SceneWindow			ui;
	SceneItem				*m_pItemParent;
	SceneEditor				*m_pEditor;

protected:
	void enterEvent(QEvent *event) override;
	void closeEvent(QCloseEvent *event) override;
	void showEvent(QShowEvent *event) override;

public:
	explicit SceneItemWindow(QWidget *parent);
	virtual ~SceneItemWindow();

	void RefreshObjectList() const;

	bool FillData(Item *item);

private slots:
	void OkButton_clicked();
	void BgColorButton_clicked();
	void ObjectList_ItemClicked(QListWidgetItem *item) const;

	void snapXBox_valueChanged(int i);
	void snapYBox_valueChanged(int i);

	void DrawGrid_stateChanged(int state);

};

