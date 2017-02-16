#include "Scene.h"

#include <QVector>

#include <ResourceView.h>
#include <Item.h>
#include <Object.h>
#include <Sprite.h>
#include <Texture.h>

Scene::Scene(QWidget * parent, QStandardItem * item, const QString & itemName)
	: Item(parent, item, itemName)
{
	ui.setupUi(this);

	// (HWND)QWidget::winId();

	RefreshObjectList();

}

Scene::~Scene()
{
	ui.objectList->clear();
}

void Scene::RefreshObjectList()
{
	ui.objectList->clear();

	QVector<Item*> items = ResourceView::Get()->GetItemsByType(Item::OBJECT);

	for (int i = 0; i < items.size(); ++i)
	{
		Object *obj = reinterpret_cast<Object*>(items[i]);
		QListWidgetItem *item = new QListWidgetItem;

		item->setText(obj->GetName());
		item->setIcon(QIcon(ResourceView::Get()->GetMainDir() + obj->GetSprite()->GetTexture()->GetPath()));

		ui.objectList->addItem(item);
	}
}
