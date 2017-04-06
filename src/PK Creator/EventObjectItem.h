#pragma once

#include <QSharedPointer>

class QStandardItem;
class QCloseEvent;
class QWidget;

class EventObjectItemWindow;

class EventObjectItem
{
public:
	enum Type
	{
		CREATE,
		DESTROY,
		PULSE,
		RENDER
	};

protected:
	QStandardItem							*m_item;

	QSharedPointer<EventObjectItemWindow>	m_pItemWnd;

	friend									EventObjectItemWindow;

public:
	EventObjectItem(Type type, QStandardItem *item);
	~EventObjectItem();

	void SetItem(QStandardItem *listItem) { m_item = listItem; }
	QStandardItem *GetItem() const { return m_item; }

	void Load(QDataStream *const dataStream);
	void Save(QDataStream *const dataStream) const;

	void Show(QWidget *parent);
	void Close();
};

