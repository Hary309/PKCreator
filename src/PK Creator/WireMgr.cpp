/*
*********************************************************************
* File          : WireMgr.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "WireMgr.h"

#include <SFML/Graphics.hpp>

#include <Widget.h>

#include <Wire.h>
#include <WireData.h>
#include <WireExec.h>

#include <QCursor>

WireMgr::WireMgr(NodeMgr *parent)
	: m_pNodeMgr(parent)
{
	m_pCurrentLine = nullptr;
}

WireMgr::~WireMgr()
{
}

WireData *WireMgr::ConnectData(sf::Vector2f startPos, ConnectTo type, VisualWidget *widget)
{
	if (!m_pCurrentLine)
	{
		m_type = type;

		auto wire = QSharedPointer<WireData>(new WireData());

		if (type == START)
		{
			wire->SetStartPos(startPos);
			wire->SetEndPos(m_cursorPos);
			wire->m_pStartWidget = widget;
		}
		else if (type == END)
		{
			wire->SetStartPos(m_cursorPos);
			wire->SetEndPos(startPos);
			wire->m_pEndWidget = widget;
		}

		m_pCurrentLine = wire.data();

		m_dataWires.push_back(wire.dynamicCast<WireData>());

		printf("New line from: (%f, %f)\n", startPos.x, startPos.y);

		return wire.data();
	}

	if (m_pCurrentLine->GetType() == Wire::EXEC || type == m_type)
	{
		Disconnect(m_pCurrentLine);
		m_pCurrentLine = nullptr;

		return nullptr;
	}

	WireData *wire = reinterpret_cast<WireData*>(m_pCurrentLine);

	if (widget)
	{
		auto widgetParent = widget->GetParent();

		if (wire->m_pEndWidget)
		{
			if (wire->m_pEndWidget->GetParent() == widgetParent)
			{
				Disconnect(m_pCurrentLine);
				m_pCurrentLine = nullptr;

				return nullptr;
			}
		}
		else if (wire->m_pStartWidget)
		{
			if (wire->m_pStartWidget->GetParent() == widgetParent)
			{
				Disconnect(m_pCurrentLine);
				m_pCurrentLine = nullptr;

				return nullptr;
			}
		}
	}

	if (type == START)
	{
		wire->SetStartPos(startPos);
		wire->m_pStartWidget = widget;
	}
	else if (type == END)
	{
		wire->SetEndPos(startPos);
		wire->m_pEndWidget = widget;
	}

	m_pCurrentLine = nullptr;

	wire->Connected();

	printf("Connected to: (%f, %f)\n", startPos.x, startPos.y);

	return wire;
}

WireExec *WireMgr::ConnectExec(sf::Vector2f startPos, ConnectTo type, VisualNode *node)
{
	if (!m_pCurrentLine)
	{
		m_type = type;

		auto wire = QSharedPointer<WireExec>(new WireExec());

		if (type == START)
		{
			wire->SetStartPos(startPos);
			wire->SetEndPos(m_cursorPos);
			wire->m_pStartNode = node;
		}
		else if (type == END)
		{
			wire->SetStartPos(m_cursorPos);
			wire->SetEndPos(startPos);
			wire->m_pEndNode = node;
		}

		m_pCurrentLine = wire.data();

		m_execWires.push_back(wire.dynamicCast<WireExec>());

		printf("New line from: (%f, %f)\n", startPos.x, startPos.y);

		return wire.data();
	}

	if (m_pCurrentLine->GetType() == Wire::DATA || type == m_type)
	{
		Disconnect(m_pCurrentLine);
		m_pCurrentLine = nullptr;

		return nullptr;
	}

	WireExec *wire = reinterpret_cast<WireExec*>(m_pCurrentLine);

	if (wire->m_pEndNode == node || wire->m_pStartNode == node)
	{
		Disconnect(m_pCurrentLine);
		m_pCurrentLine = nullptr;

		return nullptr;
	}

	if (type == START)
	{
		wire->SetStartPos(startPos);
		wire->m_pStartNode = node;
	}
	else if (type == END)
	{
		wire->SetEndPos(startPos);
		wire->m_pEndNode = node;
	}

	m_pCurrentLine = nullptr;

	wire->Connected();

	printf("Connected to: (%f, %f)\n", startPos.x, startPos.y);

	return wire;
}

void WireMgr::Disconnect(Wire *wire)
{
	if (!wire)
		return;

	if (wire->GetType() == Wire::DATA)
	{
		for (int i = 0; i < m_dataWires.size(); ++i)
		{
			if (m_dataWires[i] == wire)
			{
				wire->DisconnectAll();

				m_dataWires.removeAt(i);

				break;
			}
		}
	}
	else if (wire->GetType() == Wire::EXEC)
	{
		for (int i = 0; i < m_execWires.size(); ++i)
		{
			if (m_execWires[i] == wire)
			{
				wire->DisconnectAll();

				m_execWires.removeAt(i);

				break;
			}
		}
	}
}

void WireMgr::Render(sf::RenderWindow *renderer)
{
	for (auto wire : m_dataWires)
	{
		if (wire)
			wire->Render(renderer);
	}

	for (auto wire : m_execWires)
	{
		if (wire)
			wire->Render(renderer);
	}
}

void WireMgr::Event(sf::Event *e)
{
	if (e->type == sf::Event::MouseMoved)
	{
		m_cursorPos = sf::Vector2f(e->mouseMove.x, e->mouseMove.y);

		if (m_pCurrentLine)
		{
			if (m_type == START)
				m_pCurrentLine->SetEndPos(m_cursorPos);
			else if (m_type == END)
				m_pCurrentLine->SetStartPos(m_cursorPos);
		}
	}
	else if (e->type == sf::Event::MouseButtonPressed)
	{
		if (e->mouseButton.button == sf::Mouse::Right)
		{
			Disconnect(m_pCurrentLine);
			m_pCurrentLine = nullptr;
		}
	}

}
