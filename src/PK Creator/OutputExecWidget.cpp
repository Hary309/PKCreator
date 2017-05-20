/*
*********************************************************************
* File          : OutputExecWidget.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "OutputExecWidget.h"

#include <SFML/Graphics.hpp>

#include <BlueprintEditor.h>
#include <NodeMgr.h>
#include <VisualNode.h>
#include <WireMgr.h>
#include <Wire.h>
#include <WireExec.h>
#include <Widget.h>
#include <Node.h>

OutputExecWidget::OutputExecWidget(VisualNode *parent, Widget *data, sf::Vector2f offset)
	: VisualWidget(parent, data, offset)
{
	auto boxSize = m_pSpace->getSize();
	boxSize.x -= VisualWidget::m_verMargin;
	m_pSpace->setSize(boxSize);

	m_pPin->setPointCount(3);
	m_pPin->setRotation(90);

	data->m_color = sf::Color::White;

	m_pPin->setOutlineColor(data->m_color);

	m_waitingForWire = false;
	
	m_pWire = nullptr;
}

bool OutputExecWidget::ConnectedWith(VisualNode *node)
{
	if (!m_waitingForWire)
		return false;

	auto connected = &m_pData->m_connected;

	connected->clear();

	connected->push_back(node->GetData()->GetID());

	m_waitingForWire = false;

	return true;
}

Wire *OutputExecWidget::ConnectWire()
{
	if (m_pWire)
		m_pParent->GetNodeMgr()->GetWireMgr()->Disconnect(m_pWire);

	m_waitingForWire = true;

	m_pWire = static_cast<WireExec*>(m_pParent->GetNodeMgr()->GetWireMgr()->ConnectExec(m_pPin->getPosition(), WireMgr::START, m_pParent));

	if (!m_pWire)
		return nullptr;

	SetPin(true);

	return m_pWire;
}

void OutputExecWidget::Disconnect(Wire *wire)
{
	if (m_pWire == wire)
	{
		m_pData->m_connected.clear();

		m_pWire = nullptr;
		SetPin(false);
	}
}

void OutputExecWidget::Render(sf::RenderWindow *renderer)
{
	renderer->draw(*m_pSpace);
	renderer->draw(*m_pName);
	renderer->draw(*m_pPin);
}

void OutputExecWidget::Event(sf::Event *e)
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

void OutputExecWidget::MoveTo(sf::Vector2f pos)
{
	m_pos = pos + m_offset;

	m_pSpace->setPosition(m_pos);

	m_pPin->setPosition(m_pos + sf::Vector2f(m_pSpace->getSize().x - m_verMargin - m_pPin->getRadius(), m_height / 2));

	sf::FloatRect bounds = m_pName->getLocalBounds();
	m_pName->setPosition(sf::Vector2f(m_pPin->getPosition().x - m_pPin->getRadius() * 1.5f - bounds.width - m_verMargin, m_pos.y + m_height / 2));

	if (m_pWire)
		m_pWire->SetStartPos(m_pPin->getPosition());
}
