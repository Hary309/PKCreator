#include "ObjectItem.h"

#include <EventItem.h>
#include <SpriteItem.h>
#include <ResourceView.h>
#include <ObjectItemWindow.h>

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
	for (int i = 0; i < m_events.size(); ++i)
	{
		if (m_events[i])
		{
			delete m_events[i];
			m_events[i] = nullptr;
		}
	}

	m_events.clear();

	if (m_pItemWnd)
	{
		delete m_pItemWnd;
		m_pItemWnd = nullptr;
	}
}

void ObjectItem::SetName(const QString &name)
{
	m_itemName = name;
	m_pTreeItem->setText(name);
}

EventItem *ObjectItem::GetEvent(int eventType)
{
	for (int i = 0; i < m_events.size(); ++i)
	{
		if (m_events[i]->GetType() == eventType)
			return m_events[i];
	}

	return nullptr;
}

EventItem * ObjectItem::GetEvent(QStandardItem *item)
{
	for (int i = 0; i < m_events.size(); ++i)
	{
		if (m_events[i]->GetItem() == item)
			return m_events[i];
	}

	return nullptr;
}

void ObjectItem::Load(QDataStream *const dataStream)
{
	Item::Load(dataStream);

	long long id;
	int size;

	*dataStream >> id >> size;

	if (id)
		m_pCurrSpr = static_cast<SpriteItem*>(ResourceView::Get()->GetItem(id));

	for (int i = 0; i < size; ++i)
	{
		int type = 0;

		*dataStream >> type;

		EventItem *eventItem = new EventItem(EventItem::Type(type), nullptr);
		eventItem->Load(dataStream);

		m_events.push_back(eventItem);
	}
}

void ObjectItem::Save(QDataStream *const dataStream)
{
	Item::Save(dataStream);

	*dataStream << (m_pCurrSpr ? m_pCurrSpr->GetID() : static_cast<long long>(-1)) << m_events.size();

	for (int i = 0; i < m_events.size(); ++i)
	{
		if (m_events[i])
		{
			m_events[i]->Save(dataStream);
		}
	}
}

void ObjectItem::Show(QWidget * wndParent)
{
	if (!m_pItemWnd)
	{
		m_pItemWnd = new ObjectItemWindow(wndParent);
		m_pItemWnd->FillData(this);
		m_pItemWnd->show();
	}
}

void ObjectItem::Close()
{
	m_pItemWnd->close();

	if (m_pItemWnd)
	{
		delete m_pItemWnd;
		m_pItemWnd = nullptr;
	}
}
