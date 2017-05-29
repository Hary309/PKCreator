/*
*********************************************************************
* File          : ObjectItem.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once
#include <Item.h>

#include <QVector>
#include "EventDefsMgr.h"

class EventObjectItem;
class SpriteItem;
class ObjectItemWindow;
class CodeGenerator;
class QTreeWidgetItem;
class Var;
class Node;

class ObjectItem : public Item
{
protected:
	SpriteItem									*m_pCurrSpr;
	QVector<QSharedPointer<EventObjectItem>>	m_events;
	QVector<QSharedPointer<Var>>				m_vars;

	bool										m_solid;
	bool										m_visible;

	friend ObjectItemWindow;

public:
	ObjectItem(QStandardItem *item, const QString &itemName);
	virtual ~ObjectItem() = default;

	void SetName(const QString &name) override;

	EventObjectItem *GetEvent(int eventType);
	EventObjectItem *GetEvent(QStandardItem *item);
	auto GetEvents() { return &m_events; }

	Var *GetVar(qint64 id) const;
	Var *GetVarByWidget(qint64 widgetID, EventDefsMgr::Type eventType) const;
	Node *GetNodeByWidget(qint64 widgetID, EventDefsMgr::Type eventType) const;
	QString GetInlineVarValue(qint64 widgetID, EventDefsMgr::Type eventType) const;

	void RemoveCollisionWith(qint64 objectid);

	auto GetVars() const { return &m_vars; }

	SpriteItem *GetSprite() const { return m_pCurrSpr; }

	void Load(QDataStream *const dataStream) override;
	void Save(QDataStream *const dataStream) override;

	void Show(QWidget *wndParent)	override;
	void Close()				 	override;

	bool IsSolid() const { return m_solid; }
	bool IsVisible() const { return m_visible; }
};
