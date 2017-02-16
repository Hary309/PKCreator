#pragma once

#include "ui_SceneWindow.h"

#include <Item.h>

class Scene : public Item
{
	Q_OBJECT

private:
	Ui::SceneWindow ui;

public:
	Scene(QWidget *parent, QStandardItem *item, const QString &itemName);
	~Scene();

	void RefreshObjectList();
};
