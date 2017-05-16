/*
*********************************************************************
* File          : ObjectItem.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "ObjectItem.h"

#include <EventObjectItem.h>
#include <SpriteItem.h>
#include <ResourceView.h>
#include <ObjectItemWindow.h>
#include <Var.h>
#include <Node.h>
#include <Widget.h>

#include <QString>

ObjectItem::ObjectItem(QStandardItem *item, const QString &itemName)
	: Item(item, itemName)
{
	ObjectItem::SetName(itemName);

	m_type = Item::OBJECT;

	m_pCurrSpr = nullptr;
}

ObjectItem::~ObjectItem()
{
}


void ObjectItem::SetName(const QString &name)
{
	m_itemName = name;
	m_pTreeItem->setText(name);
}

EventObjectItem *ObjectItem::GetEvent(int eventType)
{
	for (int i = 0; i < m_events.size(); ++i)
	{
		if (m_events[i]->GetType() == eventType)
			return m_events[i].data();
	}

	return nullptr;
}

EventObjectItem *ObjectItem::GetEvent(QStandardItem *item)
{
	for (auto e : m_events)
	{
		if (e->GetItem() == item)
			return e.data();
	}

	return nullptr;
}

Var *ObjectItem::GetVar(qint64 id) const
{
	for (auto var : m_vars)
	{
		if (var->m_id == id)
			return var.data();
	}

	return nullptr;
}

Var *ObjectItem::GetVarByWidget(qint64 widgetID, EventDefsMgr::Type eventType) const
{
	qint64 id = 0;

	EventObjectItem *e = nullptr;

	for (auto e_ : m_events)
	{
		if (e_->GetType() == eventType)
		{
			e = e_.data();
			break;
		}
	}

	if (!e)
		return nullptr;

	for (auto node : *e->GetNodes())
	{
		for (auto widget : node->m_inputs)
		{
			if (widgetID == widget->GetID())
			{
				id = node->GetID();
				break;
			}
		}

		if (id)
			break;

		for (auto widget : node->m_outputs)
		{
			if (widgetID == widget->GetID())
			{
				id = node->GetID();
				break;
			}
		}

		if (id)
			break;
	}


	if (!id)
		return nullptr;

	for (auto var : m_vars)
	{
		if (var->m_id == id)
			return var.data();
	}

	return nullptr;
}

QString ObjectItem::GetInlineVarValue(qint64 widgetID, EventDefsMgr::Type eventType) const
{
	qint64 id = 0;

	EventObjectItem *e = nullptr;

	for (auto e_ : m_events)
	{
		if (e_->GetType() == eventType)
		{
			e = e_.data();
			break;
		}
	}

	if (!e)
		return nullptr;

	for (auto node : *e->GetNodes())
	{
		if (node->GetType() == Node::INLINE_VARIABLE)
		{
			for (auto widget : node->m_inputs)
			{
				if (widgetID == widget->GetID())
				{
					return node->GetDefaultValue();
				}
			}
		}
	}

	return QString();
}

void ObjectItem::Load(QDataStream *const dataStream)
{
	Item::Load(dataStream);

	// Sprite
	long long id;

	int size;

	*dataStream >> id >> size;

	if (id)
		m_pCurrSpr = static_cast<SpriteItem*>(ResourceView::Get()->GetItem(id));

	for (int i = 0; i < size; ++i)
	{
		int type = 0;

		*dataStream >> type;

		auto eventItem = QSharedPointer<EventObjectItem>(new EventObjectItem(this, EventDefsMgr::Type(type), nullptr));
		eventItem->Load(dataStream);

		m_events.push_back(eventItem);
	}

	*dataStream >> size;

	for (int i = 0; i < size; ++i)
	{
		QString name;
		qint64 id;
		int type;

		*dataStream >> name >> id >> type;

		auto var = new Var();
		var->m_name = name;
		var->m_id = id;
		var->m_type = static_cast<DataType>(type);

		switch (var->m_type)
		{
		case DATA_INTEGER:
			*dataStream >> var->m_data.integer;
			break;
		case DATA_NUMBER:
			*dataStream >> var->m_data.number;
			break;
		case DATA_STRING:
		{		
			QString str;

			*dataStream >> str;

			var->m_data.string = new QString(str);
		} break;
		case DATA_BOOLEAN:
			*dataStream >> var->m_data.boolean;
			break;
		default: ;
		}

		m_vars.push_back(QSharedPointer<Var>(var));
	}
}

void ObjectItem::Save(QDataStream *const dataStream)
{
	Item::Save(dataStream);

	*dataStream << (m_pCurrSpr ? m_pCurrSpr->GetID() : static_cast<long long>(-1)) << m_events.size();

	for (auto e : m_events)
	{
		if (e)
		{
			e->Save(dataStream);
		}
	}

	*dataStream << m_vars.size();

	for (auto var : m_vars)
	{
		*dataStream << var->m_name << var->m_id << var->m_type;

		switch (var->m_type)
		{
		case DATA_INTEGER: 
			*dataStream << var->m_data.integer;
			break;
		case DATA_NUMBER: 
			*dataStream << var->m_data.number;
			break;
		case DATA_STRING: 
			*dataStream << *var->m_data.string;
			break;
		case DATA_BOOLEAN: 
			*dataStream << var->m_data.boolean;
			break;
		default: ;
		}
	}
}

void ObjectItem::Show(QWidget *wndParent)
{
	if (!m_pItemWnd)
	{
		m_pItemWnd = QSharedPointer<ObjectItemWindow>(new ObjectItemWindow(wndParent));

		if (!m_pItemWnd->FillData(this))
		{
			m_pItemWnd.reset();
			return;
		}

		m_pItemWnd->show();
	}
}

void ObjectItem::Close()
{
	m_pItemWnd->close();

	for (auto e : m_events)
	{
		e->Close();
	}

	m_pItemWnd.reset();
}
