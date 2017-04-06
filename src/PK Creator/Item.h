#pragma once

#include <QString>
#include <QStandardItem>
#include <QDataStream>
#include <QDateTime>
#include <QSharedPointer>

class QStandardItem;
class ItemWindow;

class Item
{
public:
	enum Type
	{
		SPRITE = 0,
		BACKGROUND,
		OBJECT,
		SCENE
	};

protected:
	qint64							m_id;
	Type							m_type;
	QString							m_itemName;

	QStandardItem					*m_pTreeItem;
	QSharedPointer <ItemWindow>		m_pItemWnd;

public:
	Item(QStandardItem *item, const QString &itemName)
		: m_itemName(itemName), m_pTreeItem(item)
	{
		// getting id from timestamp
		QDateTime currDateTime = QDateTime::currentDateTime();
		m_id = currDateTime.toSecsSinceEpoch();

		m_pItemWnd = nullptr;
	}

	virtual ~Item() { }

	const QString& GetName()			const { return m_itemName; }
	const QStandardItem *GetTreeItem()	const { return m_pTreeItem; }
	qint64 GetID()						const { return m_id; }

	Type GetType()						const { return m_type; }

	virtual void SetName(const QString &name) { m_itemName = name; }

	virtual void Load(QDataStream *const dataStream) { *dataStream >> m_id; }
	virtual void Save(QDataStream *const dataStream) { *dataStream << m_type << m_itemName << m_id; }

	virtual void Show(QWidget *wndParent) = 0;
	virtual void Close() = 0;


	static bool SavingComparison(const QSharedPointer<Item> &item1, const QSharedPointer<Item> &item2) { return item1->GetType() < item2->GetType(); }
};

