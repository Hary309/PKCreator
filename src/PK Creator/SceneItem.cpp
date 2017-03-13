#include "SceneItem.h"

#include <QDataStream>

#include <SFML/Graphics/Color.hpp>

#include <Item.h>
#include <SceneItemWindow.h>
#include <ObjectItem.h>
#include <ResourceView.h>


SceneItem::SceneItem(QStandardItem *item, const QString &itemName)
	: Item(item, itemName)
{
	SceneItem::SetName(itemName);

	m_type = SCENE;

	// default value
	m_bgColor = sf::Color(158, 158, 158).toInteger();

	m_pItemWnd = nullptr;
}

SceneItem::~SceneItem()
{
	if (m_pItemWnd)
	{
		delete m_pItemWnd;
		m_pItemWnd = nullptr;
	}
}

void SceneItem::SetName(const QString &name)
{
	m_itemName = name;
	m_pTreeItem->setText(name);
}

void SceneItem::Load(QDataStream *const dataStream)
{
	Item::Load(dataStream);
	*dataStream >> m_bgColor;

	int size = 0;

	*dataStream >> size;

	for (int i = 0; i < size; ++i)
	{
		QString name;
		QPoint pos;
	
		*dataStream >> name >> pos;

		//printf("Name: %s\n", name.toStdString().c_str());
		//printf("Pos: (%d, %d)\n", pos.x(), pos.y());

		SceneObject *sObj = new SceneObject();

		sObj->pObj = static_cast<ObjectItem*>(ResourceView::Get()->GetItem(name));
		sObj->pSpr = nullptr;
		sObj->pos = pos;

		m_objects.push_back(sObj);
	}
}

void SceneItem::Save(QDataStream *const dataStream)
{
	Item::Save(dataStream);

	*dataStream << m_bgColor;

	*dataStream << m_objects.size();

	for (auto obj : m_objects)
	{
		*dataStream << obj->pObj->GetName();
		*dataStream << obj->pos;
	}
}

void SceneItem::Show(QWidget * wndParent)
{
	if (!m_pItemWnd)
	{
		m_pItemWnd = new SceneItemWindow(wndParent);
		m_pItemWnd->FillData(this);
		m_pItemWnd->show();
	}
}

void SceneItem::Close()
{
	if (m_pItemWnd)
	{
		delete m_pItemWnd;
		m_pItemWnd = nullptr;
	}
}
