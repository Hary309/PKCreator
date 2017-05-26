/*
*********************************************************************
* File          : FontItem.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "FontItem.h"

#include <FontItemWindow.h>

FontItem::FontItem(QStandardItem *item, const QString &itemName)
	: Item(item, itemName)
{
	FontItem::SetName(itemName);

	m_type = Item::FONT;

	m_font = QFont("Arial", 8);
	m_textAlign = LEFT;
	m_color = QColor(0, 0, 0).rgb();
}

void FontItem::SetName(const QString &name)
{
	m_itemName = name;
	m_pTreeItem->setText(name);
}

QString FontItem::GetFontString()
{
	return QString::number(m_font.pointSize()) + "px " + m_font.family();
}

void FontItem::Load(QDataStream *const dataStream)
{
	Item::Load(dataStream);

	int textAlign = 0;

	*dataStream >> m_font >> m_color >> textAlign;

	m_textAlign = static_cast<TextAlign>(textAlign);
}

void FontItem::Save(QDataStream *const dataStream)
{
	Item::Save(dataStream);

	*dataStream << m_font << m_color << m_textAlign;
}

void FontItem::Show(QWidget *wndParent)
{
	if (!m_pItemWnd)
	{
		m_pItemWnd = QSharedPointer<FontItemWindow>(new FontItemWindow(wndParent));

		if (!m_pItemWnd->FillData(this))
		{
			m_pItemWnd.reset();
			return;
		}

		m_pItemWnd->show();
	}
}

void FontItem::Close()
{
	m_pItemWnd->close();
	m_pItemWnd.reset();
}
