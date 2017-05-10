/*
*********************************************************************
* File          : SceneItem.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "SceneItem.h"

#include <QDataStream>

#include <SFML/Graphics/Color.hpp>

#include <Item.h>
#include <SceneItemWindow.h>
#include <ObjectItem.h>
#include <BackgroundItem.h>
#include <ResourceView.h>

#include <CodeGenerator.h>

SceneItem::SceneItem(QStandardItem *item, const QString &itemName)
	: Item(item, itemName)
{
	SceneItem::SetName(itemName);

	m_type = SCENE;

	// default value
	m_bgColor = sf::Color(158, 158, 158).toInteger();

	m_pItemWnd = nullptr;

	m_pBackground = nullptr;
	m_tileVer = false;
	m_tileHor = false;
}

SceneItem::~SceneItem()
{
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

	qint64 bgId = 0;

	*dataStream >> bgId >> m_tileHor >> m_tileVer >> size;

	if (bgId)
		m_pBackground = static_cast<BackgroundItem*>(ResourceView::Get()->GetItem(bgId));

	for (int i = 0; i < size; ++i)
	{
		long long id;
		QPoint pos;
	
		*dataStream >> id >> pos;

		auto sObj = QSharedPointer<SceneObject>(new SceneObject());

		sObj->pObj = static_cast<ObjectItem*>(ResourceView::Get()->GetItem(id));
		sObj->pSpr = nullptr;
		sObj->pos = pos;

		m_objects.push_back(sObj);
	}
}

void SceneItem::Save(QDataStream *const dataStream)
{
	Item::Save(dataStream);

	*dataStream << m_bgColor << (m_pBackground ? m_pBackground->GetID() : static_cast<long long>(-1)) << m_tileHor << m_tileVer;

	*dataStream << m_objects.size();

	for (auto obj : m_objects)
	{
		*dataStream << obj->pObj->GetID();
		*dataStream << obj->pos;
	}
}

void SceneItem::Show(QWidget *wndParent)
{
	if (!m_pItemWnd)
	{
		m_pItemWnd = QSharedPointer<SceneItemWindow>(new SceneItemWindow(wndParent));

		if (!m_pItemWnd->FillData(this))
			m_pItemWnd.reset();

		m_pItemWnd->show();
	}
}

void SceneItem::Close()
{
	m_pItemWnd->close();

	m_pItemWnd.reset();
}
