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
	void enterEvent(QEvent *event) override;
	void closeEvent(QCloseEvent *event) override;
	void showEvent(QShowEvent *event) override;

public:
	Scene(QWidget *parent, QStandardItem *item, const QString &itemName);
	~Scene();

	void SetName(const QString &name) override;

	void RefreshObjectList() const;

	void SetSceneEditorSize(const QSize &size) const;

	void Load(QDataStream *const dataStream) override;
	void Save(QDataStream *const dataStream) override;

private slots:
	void OkButton_clicked();
	void ObjectList_ItemClicked(QListWidgetItem *item) const;
};
