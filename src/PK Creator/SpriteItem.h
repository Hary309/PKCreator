#pragma once

#include <Item.h>

class SpriteItemWindow;
class SceneEditor;

class SpriteItem : public Item
{
protected:
	QSize			m_size;
	QString			m_texPath; // relative path ("resources/timeinhex.ext")
	QPoint			m_center;

	friend			SpriteItemWindow;

public:
	SpriteItem(QStandardItem *item, const QString &itemName);
	virtual ~SpriteItem();

	QSize GetSize()			const	 { return m_size; }
	QPoint GetCenter()		const	 { return m_center; }
	QString GetTexPath()	const	 { return m_texPath; }

	void SetName(const QString &name)			override;

	void Load(QDataStream * const dataStream)	override;
	void Save(QDataStream * const dataStream)	override;

	void Show(QWidget *wndParent)				override;
	void Close()								override;
};

