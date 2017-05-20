/*
*********************************************************************
* File          : InputWidget.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "InputDataWidget.h"

#include <SFML/Graphics.hpp>

#include <Widget.h>
#include <BlueprintEditor.h>
#include <NodeMgr.h>
#include <VisualNode.h>
#include <WireMgr.h>
#include <Wire.h>
#include <WireData.h>

InputDataWidget::InputDataWidget(VisualNode *parent, Widget *data, sf::Vector2f offset)
	: VisualWidget(parent, data, offset)
{
	auto boxSize = m_pSpace->getSize();
	boxSize.x -= VisualWidget::m_verMargin;
	m_pSpace->setSize(boxSize);

	m_pWire = nullptr;
}

void InputDataWidget::ConnectedWith(VisualWidget *widget)
{
	auto connected = &m_pData->m_connected;

	connected->clear();

	connected->push_back(widget->GetData()->GetID());
}

Wire *InputDataWidget::ConnectWire()
{
	if (m_pWire)
		m_pParent->GetNodeMgr()->GetWireMgr()->Disconnect(m_pWire);

	m_pWire = static_cast<WireData*>(m_pParent->GetNodeMgr()->GetWireMgr()->ConnectData(m_pPin->getPosition(), WireMgr::END, this));

	if (!m_pWire)
		return nullptr;

	SetPin(true);
	
	return m_pWire;
}

void InputDataWidget::Disconnect(Wire *wire)
{
	if (m_pWire == wire)
	{
		m_pData->m_connected.clear();

		m_pWire = nullptr;
		SetPin(false);
	}
}

void InputDataWidget::Render(sf::RenderWindow *renderer)
{
	renderer->draw(*m_pSpace);
	renderer->draw(*m_pName);
	renderer->draw(*m_pPin);
}

void InputDataWidget::Event(sf::Event *e)
{
	VisualWidget::Event(e);

	if (e->type == sf::Event::MouseButtonPressed && (e->mouseButton.button == sf::Mouse::Left || e->mouseButton.button == sf::Mouse::Right))
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

void InputDataWidget::MoveTo(sf::Vector2f pos)
{
	m_pos = pos + m_offset;

	m_pSpace->setPosition(m_pos);

	m_pPin->setPosition(m_pos + sf::Vector2f(m_verMargin + m_pPin->getRadius(), m_height / 2));

	m_pName->setPosition(sf::Vector2f(m_pPin->getPosition().x + m_pPin->getRadius() + m_verMargin, m_pos.y + m_height / 2));

	if (m_pWire)
		m_pWire->SetEndPos(m_pPin->getPosition());
}
