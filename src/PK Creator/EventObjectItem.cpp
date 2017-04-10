#include "EventObjectItem.h"

#include <EventObjectItemWindow.h>

EventObjectItem::EventObjectItem(Type type, QStandardItem *item)
	: m_eventType(type), m_item(item)
{
	m_pItemWnd = nullptr;
}

EventObjectItem::~EventObjectItem()
{
}

void EventObjectItem::Load(QDataStream *const dataStream)
{
}

void EventObjectItem::Save(QDataStream *const dataStream) const
{
}

void EventObjectItem::Show(QWidget *parent)
{
	if (!m_pItemWnd)
	{
		m_pItemWnd = QSharedPointer<EventObjectItemWindow>(new EventObjectItemWindow(parent));

		if (!m_pItemWnd->FillData(this))
		{
			m_pItemWnd.reset();
			return;
		}

		m_pItemWnd->show();
	}
}

void EventObjectItem::Close()
{
	if (m_pItemWnd)
	{
		m_pItemWnd->close();

		m_pItemWnd.reset(nullptr);
	}
}
