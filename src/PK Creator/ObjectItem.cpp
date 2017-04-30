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
#include <CodeGenerator.h>
#include <Var.h>

#include <QString>

ObjectItem::ObjectItem(QStandardItem *item, const QString &itemName)
	: Item(item, itemName)
{
	ObjectItem::SetName(itemName);

	m_eventName << "Create Event" << "Destroy Event" << "Pulse Event" << "Render Event";

	m_type = Item::OBJECT;

	m_pCurrSpr = nullptr;
}

ObjectItem::~ObjectItem()
{
}

void ObjectItem::GenerateCode(CodeGenerator *codeGenerator)
{
	codeGenerator->GenerateObject(this);
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

		auto eventItem = QSharedPointer<EventObjectItem>(new EventObjectItem(EventObjectItem::Type(type), nullptr));
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
