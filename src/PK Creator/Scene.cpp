#include "Scene.h"

Scene::Scene(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	// (HWND)QWidget::winId();
}

Scene::~Scene()
{

}
