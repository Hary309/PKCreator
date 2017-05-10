/*
*********************************************************************
* File          : BackgroundItem.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <Item.h>

class BackgroundItemWindow;

class BackgroundItem : public Item
{
protected:
	QSize			m_size;
	QString			m_texPath;

	friend BackgroundItemWindow;

public:
	BackgroundItem(QStandardItem *item, const QString &itemName);
	~BackgroundItem();

	QSize GetSize()			const		{ return m_size; }
	QString GetTexPath()	const		{ return m_texPath; }

	void SetName(const QString &name)			override;

	void Load(QDataStream *const dataStream)	override;
	void Save(QDataStream *const dataStream)	override;

	void Show(QWidget *wndParent)				override;
	void Close()								override;
};

