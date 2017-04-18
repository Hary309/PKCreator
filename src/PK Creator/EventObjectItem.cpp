/*
*********************************************************************
* File          : EventObjectItem.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "EventObjectItem.h"

#include <EventObjectItemWindow.h>
#include <Node.h>

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
	int n = 0;

	*dataStream >> n;

	printf("number of nodes: %d\n", n);

	for (int i = 0; i < n; ++i)
	{
		auto node = QSharedPointer<Node>(new Node(QString(), sf::Vector2f()));
		node->Load(dataStream);

		m_nodes.push_back(node);
	}
}

void EventObjectItem::Save(QDataStream *const dataStream) const
{
	*dataStream << m_eventType << m_nodes.size();

	for (auto node : m_nodes)
	{
		node->Save(dataStream);
	}
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
