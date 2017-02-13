#pragma once

#include <QMainWindow>
#include "ui_SceneWindow.h"

class Scene : public QMainWindow
{
	Q_OBJECT
private:
	Ui::SceneWindow ui;

public:
	Scene(QWidget *parent = 0);
	~Scene();
};
