/*
*********************************************************************
* File          : EventObjectItem.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <QSharedPointer>
#include <QVector>

class QStandardItem;
class QCloseEvent;
class QWidget;

class EventObjectItemWindow;

class BlueprintEditor;
class Node;

class EventObjectItem
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
	QStandardItem							*m_item;

	Type									m_eventType;

	QVector<QSharedPointer<Node>>			m_nodes;

	QSharedPointer<EventObjectItemWindow>	m_pItemWnd;

	friend									BlueprintEditor;

public:
	EventObjectItem(Type type, QStandardItem *item);
	~EventObjectItem();

	void SetItem(QStandardItem *listItem) { m_item = listItem; }
	QStandardItem *GetItem() const { return m_item; }

	void Load(QDataStream *const dataStream);
	void Save(QDataStream *const dataStream) const;

	void Show(QWidget *parent);
	void Close();

	Type GetType() const { return m_eventType; }
};

