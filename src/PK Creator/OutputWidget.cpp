/*
*********************************************************************
* File          : OutputWidget.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "OutputWidget.h"

#include <SFML/Graphics.hpp>

#include <Widget.h>
#include <NodeMgr.h>
#include <BlueprintEditor.h>
#include <VisualNode.h>
#include <WireMgr.h>
#include <Wire.h>
#include <WireData.h>

OutputWidget::OutputWidget(VisualNode *parent, Widget *data, sf::Vector2f offset)
	: VisualWidget(parent, data, offset)
{
}

OutputWidget::~OutputWidget()
{
}

void OutputWidget::ConnectedWith(VisualWidget *widget)
{
	auto connected = &m_pData->m_connected;

	for (auto widget_ : *connected)
	{
		if (widget_ == widget->GetData()->GetID())
			return;
	}

	connected->push_back(widget->GetData()->GetID());
}

WireData *OutputWidget::ConnectWire()
{
	WireData *wire = static_cast<WireData*>(m_pParent->GetNodeMgr()->GetWireMgr()->ConnectData(m_pPin->getPosition(), WireMgr::START, this));

	if (!wire)
		return nullptr;

	SetPin(true);
	wire->m_pStartWidget = this;

	m_wires.push_back(wire);

	return wire;
}

void OutputWidget::Disconnect(Wire *wire)
{
	auto connected = &m_pData->m_connected;

	for (int i = 0; i < connected->size(); ++i)
	{
		if (reinterpret_cast<WireData*>(wire)->m_pEndWidget)
		{
			if (connected->at(i) == reinterpret_cast<WireData*>(wire)->m_pEndWidget->GetData()->GetID())
			{
				connected->removeAt(i);
				break;
			}
		}
	}

	for (int i = 0; i < m_wires.size(); ++i)
	{
		if (m_wires[i] == wire)
		{
			m_wires.removeAt(i);
			break;
		}
	}

	if (m_wires.empty())
		SetPin(false);
}

void OutputWidget::Render(sf::RenderWindow *renderer)
{
	renderer->draw(*m_pSpace);
	renderer->draw(*m_pName);
	renderer->draw(*m_pPin);
}

void OutputWidget::Event(sf::Event *e)
{
	if (e->type == sf::Event::MouseButtonPressed && e->mouseButton.button == sf::Mouse::Left)
	{
		sf::Vector2f viewOffset = m_pParent->GetNodeMgr()->GetBpEditor()->GetViewOffset();
		float scale = m_pParent->GetNodeMgr()->GetBpEditor()->GetScale();

		auto cursorPos = sf::Vector2f(e->mouseButton.x, e->mouseButton.y) * scale - viewOffset;
		auto spacePos = m_pSpace->getPosition();
		auto spaceSize = m_pSpace->getSize();

		if (cursorPos.x > spacePos.x &&
			cursorPos.y > spacePos.y &&
			cursorPos.x < spacePos.x + spaceSize.x &&
			cursorPos.y < spacePos.y + spaceSize.y)
		{
			ConnectWire();
		}
	}
}

void OutputWidget::MoveTo(sf::Vector2f pos)
{
	m_pos = pos + m_offset;

	m_pSpace->setPosition(m_pos);

	m_pPin->setPosition(m_pos + sf::Vector2f(m_pSpace->getSize().x - m_verMargin, m_height / 2));

	sf::FloatRect bounds = m_pName->getLocalBounds();
	m_pName->setPosition(sf::Vector2f(m_pPin->getPosition().x - m_pPin->getSize().x * 1.5f - bounds.width - m_verMargin, m_pos.y + m_height / 2));

	for (auto wire : m_wires)
	{
		if (wire)
		{
			wire->SetStartPos(m_pPin->getPosition());
		}
	}
}
