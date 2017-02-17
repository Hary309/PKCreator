#pragma once

#include "ui_SceneWindow.h"

#include <Item.h>

class SceneEditor;

class Scene : public Item
{
	Q_OBJECT

	friend SceneEditor;

private:
	Ui::SceneWindow		ui;
	SceneEditor			*m_pSceneEditor;

protected:
	virtual void enterEvent(QEvent *event) override;
	virtual void closeEvent(QCloseEvent *event) override;
	virtual void showEvent(QShowEvent *event) override;

public:
	Scene(QWidget *parent, QStandardItem *item, const QString &itemName);
	~Scene();

	void SetName(const QString &name);

	void RefreshObjectList();

private slots:
	void OkButton_clicked();
};
