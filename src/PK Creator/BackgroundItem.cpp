/*
*********************************************************************
* File          : BackgroundItem.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "BackgroundItem.h"

#include <BackgroundItemWindow.h>

BackgroundItem::BackgroundItem(QStandardItem *item, const QString &itemName)
	: Item(item, itemName)
{
	BackgroundItem::SetName(itemName);

	m_type = Item::BACKGROUND;

	m_size = QSize(0, 0);

	m_pItemWnd = nullptr;
}

BackgroundItem::~BackgroundItem()
{
}

void BackgroundItem::SetName(const QString &name)
{
	m_itemName = name;
	m_pTreeItem->setText(name);
}

void BackgroundItem::Load(QDataStream *const dataStream)
{
	Item::Load(dataStream);

	*dataStream >> m_size >> m_texPath;
}

void BackgroundItem::Save(QDataStream *const dataStream)
{
	Item::Save(dataStream);

	*dataStream << m_size << m_texPath;
}

void BackgroundItem::Show(QWidget *wndParent)
{
	if (!m_pItemWnd)
	{
		m_pItemWnd = QSharedPointer<BackgroundItemWindow>(new BackgroundItemWindow(wndParent));

		if (!m_pItemWnd->FillData(this))
			m_pItemWnd.reset();

		m_pItemWnd->show();
	}
}

void BackgroundItem::Close()
{
	m_pItemWnd->close();

	m_pItemWnd.reset();
}
