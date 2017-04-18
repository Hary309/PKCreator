/*
*********************************************************************
* File          : InputWidget.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "InputWidget.h"

#include <SFML/Graphics.hpp>

#include <Widget.h>
#include <NodeMgr.h>
#include <VisualNode.h>
#include <WireMgr.h>
#include <Wire.h>
#include <WireData.h>

InputWidget::InputWidget(VisualNode *parent, Widget *data, sf::Vector2f offset)
	: VisualWidget(parent, data, offset)
{
	auto boxSize = m_pSpace->getSize();
	boxSize.x -= VisualWidget::m_verMargin;
	m_pSpace->setSize(boxSize);

	m_pWire = nullptr;
}


InputWidget::~InputWidget()
{
}

void InputWidget::ConnectedWith(VisualWidget *widget)
{
	auto connected = &m_pData->m_connected;

	connected->clear();

	connected->push_back(widget->GetData()->GetID());
}

WireData *InputWidget::ConnectWire()
{
	if (m_pWire)
		m_pParent->GetNodeMgr()->GetWireMgr()->Disconnect(m_pWire);

	m_pWire = static_cast<WireData*>(m_pParent->GetNodeMgr()->GetWireMgr()->ConnectData(m_pPin->getPosition(), WireMgr::END, this));

	if (!m_pWire)
		return nullptr;

	SetPin(true);
	
	return m_pWire;
}

void InputWidget::Disconnect(Wire *wire)
{
	if (m_pWire == wire)
	{
		m_pData->m_connected.clear();

		m_pWire = nullptr;
		SetPin(false);
	}
}

void InputWidget::Render(sf::RenderWindow *renderer)
{
	renderer->draw(*m_pSpace);
	renderer->draw(*m_pName);
	renderer->draw(*m_pPin);
}

void InputWidget::Event(sf::Event *e)
{
	if (e->type == sf::Event::MouseButtonPressed && (e->mouseButton.button == sf::Mouse::Left || e->mouseButton.button == sf::Mouse::Right))
	{
		auto cursorPos = sf::Vector2f(e->mouseButton.x, e->mouseButton.y);
		auto spacePos = m_pSpace->getPosition();
		auto spaceSize = m_pSpace->getSize();

		if (cursorPos.x > spacePos.x &&
			cursorPos.y > spacePos.y &&
			cursorPos.x < spacePos.x + spaceSize.x &&
			cursorPos.y < spacePos.y + spaceSize.y)
		{
			if (e->mouseButton.button == sf::Mouse::Left)
			{
				ConnectWire();
			}
			else if (e->mouseButton.button == sf::Mouse::Right)
			{
				if (m_pWire)
					m_pParent->GetNodeMgr()->GetWireMgr()->Disconnect(m_pWire);
			}
		}
	}
}

void InputWidget::MoveTo(sf::Vector2f pos)
{
	m_pos = pos + m_offset;

	m_pSpace->setPosition(m_pos);

	m_pPin->setPosition(m_pos + sf::Vector2f(m_verMargin + m_pPin->getSize().x, m_height / 2));

	m_pName->setPosition(sf::Vector2f(m_pPin->getPosition().x + m_pPin->getSize().x + m_verMargin, m_pos.y + m_height / 2));

	if (m_pWire)
		m_pWire->SetEndPos(m_pPin->getPosition());
}
