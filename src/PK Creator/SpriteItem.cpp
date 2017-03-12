#include "SpriteItem.h"

#include <QStandardItemModel>
#include <QFileDialog>

#include <SpriteItemWindow.h>

SpriteItem::SpriteItem( QStandardItem *item, const QString &itemName)
	: Item(item, itemName)
{
	SpriteItem::SetName(itemName);

	m_type = Item::SPRITE;
	
	m_size = QSize(0, 0);
	m_center = QPoint(0, 0);

	m_pItemWnd = nullptr;
}


SpriteItem::~SpriteItem()
{
	if (m_pItemWnd)
	{
		delete m_pItemWnd;
		m_pItemWnd = nullptr;
	}
}

void SpriteItem::SetName(const QString &name)
{
	m_itemName = name;
	m_pTreeItem->setText(name);
}

void SpriteItem::Load(QDataStream *const dataStream)
{
	Item::Load(dataStream);

	*dataStream >> m_size >> m_texPath >> m_center;
}

void SpriteItem::Save(QDataStream *const dataStream)
{
	Item::Save(dataStream);

	*dataStream << m_size << m_texPath << m_center;
}

void SpriteItem::Show(QWidget *wndParent)
{
	if (!m_pItemWnd)
	{
		m_pItemWnd = new SpriteItemWindow(wndParent);
		if (!m_pItemWnd->FillData(this))
		{
			delete m_pItemWnd;
			m_pItemWnd = nullptr;

			return;
		}
		m_pItemWnd->show();
	}
}

void SpriteItem::Close()
{
	m_pItemWnd->close();

	if (m_pItemWnd)
	{
		delete m_pItemWnd;
		m_pItemWnd = nullptr;
	}
}
