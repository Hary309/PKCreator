#include "EventItem.h"

#include <QMessageBox>

#include <EventItemWindow.h>

EventItem::EventItem(Type type, QStandardItem *item)
	: m_type(type), m_item(item)
{
	m_pItemWnd = nullptr;
}

EventItem::~EventItem()
{
	if (m_pItemWnd)
	{
		delete m_pItemWnd;
		m_pItemWnd = nullptr;
	}
}

void EventItem::Load(QDataStream *const dataStream)
{
	*dataStream >> m_code;
}

void EventItem::Save(QDataStream *const dataStream) const
{
	*dataStream << m_type << m_code;
}

void EventItem::Show(QWidget *parent)
{
	if (!m_pItemWnd)
	{
		m_pItemWnd = new EventItemWindow(parent);
		m_pItemWnd->FillData(this);
		m_pItemWnd->show();
	}
}

void EventItem::Close()
{
	if (m_pItemWnd)
	{
		delete m_pItemWnd;
		m_pItemWnd = nullptr;
	}
}
