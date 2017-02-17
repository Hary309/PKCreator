#include "Scene.h"

#include <QVector>
#include <QMouseEvent>
#include <QMessageBox>

#include <ResourceView.h>
#include <Item.h>
#include <Object.h>
#include <Sprite.h>
#include <Texture.h>

#include <SceneEditor.h>

Scene::Scene(QWidget *parent, QStandardItem *item, const QString &itemName)
	: Item(parent, item, itemName)
{
	ui.setupUi(this);

	SetName(itemName);

	RefreshObjectList();

	m_pSceneEditor = new SceneEditor(this);
	m_pSceneEditor->show();

	m_pSceneEditor->setWindowTitle(itemName + QString(" - Scene Editor"));

	connect(ui.okButton, &QPushButton::clicked, this, &Scene::OkButton_clicked);
}

Scene::~Scene()
{
	ui.objectList->clear();

	if (m_pSceneEditor)
	{
		delete m_pSceneEditor;
		m_pSceneEditor = nullptr;
	}
}

void Scene::SetName(const QString &name)
{
	m_itemName = name;
	setWindowTitle(name);
	ui.nameEdit->setText(name);
	m_pItem->setText(name);
}

void Scene::enterEvent(QEvent *e)
{
	Item::enterEvent(e);

	RefreshObjectList();
}

void Scene::closeEvent(QCloseEvent *e)
{
	if (m_pSceneEditor)
		m_pSceneEditor->hide();
}

void Scene::showEvent(QShowEvent *e)
{
	if (m_pSceneEditor)
		m_pSceneEditor->show();
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
		if (obj->GetSprite())
			if (obj->GetSprite()->GetTexture())
				item->setIcon(QIcon(ResourceView::Get()->GetMainDir() + obj->GetSprite()->GetTexture()->GetPath()));

		ui.objectList->addItem(item);
	}
}

void Scene::OkButton_clicked()
{
	QString name = ui.nameEdit->text();

	if (ResourceView::Get()->IsNameExists(name) && name != m_itemName)
	{
		QMessageBox::information(this, "PK Creator", "This name already exists!");
		return;
	}

	SetName(name);
	hide();

	m_pSceneEditor->hide();
}
