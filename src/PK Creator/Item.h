#pragma once

#include <QString>
#include <QDialog>
#include <QStandardItem>

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
		: QDialog(parent), m_pItem(item), m_itemName(itemName)
	{
		setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	}

	virtual ~Item() { }

	const QString& GetName()		{ return m_itemName; }
	const QStandardItem *GetItem()	{ return m_pItem; }

	Type GetType()					{ return m_type; }

	virtual void SetName(const QString &name) = 0;
};

