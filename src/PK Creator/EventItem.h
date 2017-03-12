#pragma once

#include <CodeEditorWindow.h>

class QStandardItem;
class QCloseEvent;
class QWidget;

class EventItemWindow;

class EventItem
{
public:
	enum Type
	{
		CREATE = 0,
		DESTROY,
		PULSE,
		RENDER
	};

protected:
	Type			m_type;
	QString			m_code;

	QStandardItem	*m_item;

	EventItemWindow	*m_pItemWnd;

	friend			EventItemWindow;

public:
	EventItem(Type type, QStandardItem *item);
	~EventItem();

	Type GetType() const { return m_type; }
	
	void SetItem(QStandardItem *listItem) { m_item = listItem; }
	QStandardItem *GetItem() const { return m_item; }

	void Load(QDataStream * const dataStream);
	void Save(QDataStream * const dataStream) const;

	void Show(QWidget *parent);
	void Close();
};

