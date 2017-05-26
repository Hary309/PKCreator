/*
*********************************************************************
* File          : FontItem.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <Item.h>

class FontItemWindow;

class FontItem : public Item
{
public:
	enum TextAlign
	{
		LEFT,
		CENTER,
		RIGHT
	};

private:
	QFont			m_font;
	QColor			m_color;
	TextAlign		m_textAlign;

	friend FontItemWindow;

public:
	FontItem(QStandardItem *item, const QString &itemName);
	~FontItem() = default;

	void SetName(const QString &name) override;

	QString GetFontString();
	QString GetFontFamily() const { return m_font.family(); }
	int GetSize() const { return m_font.pointSize(); }
	QColor GetColor() const { return m_color; }
	TextAlign GetTextAlign() const { return m_textAlign; }

	void Load(QDataStream *const dataStream) override;
	void Save(QDataStream *const dataStream) override;

	void Show(QWidget *wndParent)	override;
	void Close()				 	override;
};

