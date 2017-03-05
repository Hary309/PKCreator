#include "Scene.h"

#include <QVector>
#include <QMouseEvent>
#include <QMessageBox>

#include <ResourceView.h>
#include <Item.h>
#include <Object.h>
#include <Sprite.h>

#include <SceneEditor.h>
#include <TextureMgr.h>

Scene::Scene(QWidget *parent, QStandardItem *item, const QString &itemName)
	: Item(parent, item, itemName)
{
	ui.setupUi(this);

	Scene::SetName(itemName);

	m_type = SCENE;

	ui.nameEdit->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9]{1,24}")));

	m_pSceneEditor = new SceneEditor(this);

	m_pSceneEditor->setWindowTitle(itemName + QString(" - Scene Editor"));

	RefreshObjectList();

	connect(ui.okButton, &QPushButton::clicked, this, &Scene::OkButton_clicked);
	connect(ui.objectList, &QListWidget::itemClicked, this, &Scene::ObjectList_ItemClicked);
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
		m_pSceneEditor->close();
}

void Scene::showEvent(QShowEvent *e)
{
	if (m_pSceneEditor)
		m_pSceneEditor->show();
}

void Scene::RefreshObjectList() const
{
	if (!m_pSceneEditor)
		return;

	if (!m_pSceneEditor->GetTexMgr())
		return;

	ui.objectList->clear();

	QVector<Item*> items = ResourceView::Get()->GetItemsByType(Item::OBJECT);
	
	printf("Size: %d\n", items.size());

	for (int i = 0; i < items.size(); ++i)
	{
		Object *obj = reinterpret_cast<Object*>(items[i]);
		QListWidgetItem *item = new QListWidgetItem;

		if (obj->GetSprite())
		{
			Sprite *spr = obj->GetSprite();

			item->setText(obj->GetName());

			if (spr)
			{
				item->setIcon(QIcon(ResourceView::Get()->GetMainDir() + spr->GetTexPath()));

				ui.objectList->addItem(item);
			}
		}
	}
}

void Scene::SetSceneEditorSize(const QSize &size) const
{
	if (m_pSceneEditor)
	{
		m_pSceneEditor->setFixedSize(size);
	}
}

void Scene::Load(QDataStream * const dataStream)
{

}

void Scene::Save(QDataStream * const dataStream)
{
	Item::Save(dataStream);
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

void Scene::ObjectList_ItemClicked(QListWidgetItem *item) const
{
	if (ResourceView::Get()->GetItem(item->text())->GetType() == Item::OBJECT)
	{
		Object *objItem = static_cast<Object*>(ResourceView::Get()->GetItem(item->text()));

		QString path = ResourceView::Get()->GetMainDir() + objItem->GetSprite()->GetTexPath();
	
		m_pSceneEditor->GetTexMgr()->LoadTexture(objItem->GetSprite());

		m_pSceneEditor->SetCurrObject(objItem->GetSprite()->GetName());
	}
}
