#pragma once

#include <QString>
#include <QDialog>
#include <QStandardItem>
#include <QDataStream>

class QStandardItem;

class Item : public QDialog
{
public:
	enum Type
	{
		TEXTURE = 0,
		SPRITE,
		BACKGROUND,
		OBJECT,
		SCENE
	};

protected:
	Type			m_type;
	QString			m_itemName;

	QStandardItem	*m_pItem;

public:
	Item(QWidget *parent, QStandardItem *item, const QString &itemName) 
		: QDialog(parent), m_itemName(itemName), m_pItem(item)
	{
		setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	}

	virtual ~Item() { }

	const QString& GetName()		const { return m_itemName; }
	const QStandardItem *GetItem()	const { return m_pItem; }

	Type GetType() const			{ return m_type; }

	virtual void SetName(const QString &name) { m_itemName = name; }
	virtual void Load(QDataStream *const dataStream){}

	virtual void Save(QDataStream *const dataStream) { *dataStream << m_type << m_itemName; }

	bool operator<(const Item *item) const { return m_type < item->GetType(); }
};

