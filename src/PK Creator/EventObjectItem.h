/*
*********************************************************************
* File          : EventObjectItem.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <EventDefsMgr.h>

class QStandardItem;
class QCloseEvent;
class QWidget;
class ObjectItem;
class EventObjectItemWindow;

class BlueprintEditor;
class Node;
class ObjectItem;
class ObjectItemWindow;
class HTML5Generator;

class EventObjectItem
{
protected:
	ObjectItem								*m_pParent;
	QStandardItem							*m_item;

	EventDefsMgr::Type						m_eventType;

	QVector<QSharedPointer<Node>>			m_nodes;

	QSharedPointer<EventObjectItemWindow>	m_pItemWnd;

	bool									m_new;

	// for collision type
	qint64									m_idCurrentObject; // this
	qint64									m_idCollideWith;

	friend BlueprintEditor;
	friend ObjectItemWindow;
	friend HTML5Generator;
	friend ObjectItem;

public:
	EventObjectItem(ObjectItem *parent, EventDefsMgr::Type type, QStandardItem *item);
	~EventObjectItem();

	void SetItem(QStandardItem *listItem) { m_item = listItem; }
	QStandardItem *GetItem() const { return m_item; }

	void Load(QDataStream *const dataStream);
	void Save(QDataStream *const dataStream) const;

	auto GetNodes() const { return &m_nodes; }
	Node *GetFirstNode() const { return m_nodes.first().data(); }
	Node *GetNode(qint64 id) const;

	void Show(QWidget *parent);
	void Close();

	void SetAsNew() { m_new = true; }

	ObjectItem *GetParent() const { return m_pParent; }

	EventDefsMgr::Type GetType() const { return m_eventType; }
};

