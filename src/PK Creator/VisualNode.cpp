/*
*********************************************************************
* File          : VisualNode.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "VisualNode.h"

#include <NodeMgr.h>
#include <Node.h>

#include <WireMgr.h>
#include <Wire.h>
#include <WireExec.h>

#include <Widget.h>
#include <InputWidget.h>
#include <OutputWidget.h>

#include <SFML/Graphics.hpp>

VisualNode::VisualNode(NodeMgr *nodeMgr, Node *data, sf::Color defaultColor, int width)
	: m_pNodeMgr(nodeMgr), m_pData(data), m_defaultColor(defaultColor), m_boxWidth(width), m_pFont(m_pNodeMgr->GetFont())
{
	m_pHeader = QSharedPointer<sf::RectangleShape>(new sf::RectangleShape(sf::Vector2f(m_boxWidth, 20)));
	m_pBody = QSharedPointer<sf::RectangleShape>(new sf::RectangleShape(sf::Vector2f(m_boxWidth, 0)));

	m_pHeader->setFillColor(m_defaultColor);
	m_pBody->setFillColor(sf::Color(0x424242FF));

	m_pHeader->setOutlineColor(m_defaultColor);
	m_pHeader->setOutlineThickness(2.f);

	m_pBody->setOutlineColor(m_defaultColor);
	m_pBody->setOutlineThickness(2.f);

	m_pTitle = QSharedPointer<sf::Text>(new sf::Text());
	m_pTitle->setFont(*m_pFont);
	m_pTitle->setCharacterSize(15);
	m_pTitle->setString(m_pData->m_name.toStdString());

	m_moving = false;

	for (int i = 0; i < m_pData->m_inputs.size(); ++i)
	{
		auto widget = m_pData->m_inputs[i];

		if (widget)
		{
			auto offset = sf::Vector2f(VisualWidget::m_verMargin, i == 0 ? 20.f + VisualWidget::m_horMargin : 20.f + (i + 1) * VisualWidget::m_horMargin + i * m_visualInputs.last()->GetHeight());

			auto inputWidget = QSharedPointer<InputWidget>(new InputWidget(this, widget.data(), offset));

			m_visualInputs.push_back(inputWidget);

			auto bodySize = m_pBody->getSize();

			if (bodySize.y < offset.y)
				m_pBody->setSize(sf::Vector2f(bodySize.x, offset.y + VisualWidget::m_horMargin));
		}
	}

	for (int i = 0; i < m_pData->m_outputs.size(); ++i)
	{
		auto widget = m_pData->m_outputs[i];

		if (widget)
		{
			auto offset = sf::Vector2f(m_boxWidth / 2, i == 0 ? 20.f + VisualWidget::m_horMargin : 20.f + (i + 1) * VisualWidget::m_horMargin + i * m_visualOutputs.last()->GetHeight());

			auto outputWidget = QSharedPointer<OutputWidget>(new OutputWidget(this, widget.data(), offset));
			
			m_visualOutputs.push_back(outputWidget);

			auto bodySize = m_pBody->getSize();

			if (bodySize.y < offset.y)
				m_pBody->setSize(sf::Vector2f(bodySize.x, offset.y + VisualWidget::m_horMargin));
		}
	}

	m_pWireExecTo = nullptr;
	m_pWireExecFrom = nullptr;

	AddExecFrom();
	AddExecTo();

	MoveTo(data->m_pos);
}

VisualNode::~VisualNode()
{
}

void VisualNode::Render(sf::RenderWindow *renderer)
{
	renderer->draw(*m_pHeader);
	renderer->draw(*m_pBody);
	renderer->draw(*m_pTitle);

	if (m_pShapeExecFrom)
		renderer->draw(*m_pShapeExecFrom);

	if (m_pShapeExecTo)
		renderer->draw(*m_pShapeExecTo);

	for (auto widget : m_visualInputs)
	{
		if (widget)
			widget->Render(renderer);
	}

	for (auto widget : m_visualOutputs)
	{
		if (widget)
			widget->Render(renderer);
	}
}

void VisualNode::Event(sf::Event *e)
{
	switch (e->type)
	{
		case sf::Event::MouseButtonPressed:
		{
			if (e->mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2f nodePos = m_pData->m_pos;
				sf::Vector2f size = m_pHeader->getSize();
				sf::Vector2f mousePos = sf::Vector2f(e->mouseButton.x, e->mouseButton.y);


				if (mousePos.x > nodePos.x &&
					mousePos.x < nodePos.x + size.x &&
					mousePos.y > nodePos.y &&
					mousePos.y < nodePos.y + size.y)
				{
					if (mousePos.x < nodePos.x + 14.f)
					{
						Connect(ExecType::ExecFrom);
					}
					else if (mousePos.x > nodePos.x + m_boxWidth - 14.f)
					{
						Connect(ExecType::ExecTo);
					}
					else
					{
						m_moving = true;

						m_mouseOffset = mousePos - nodePos;

						MoveTo(mousePos - m_mouseOffset);
					}
				}
			}
		} break;
		case sf::Event::MouseButtonReleased:
		{
			m_moving = false;
			m_mouseOffset = sf::Vector2f(0.f, 0.f);
		} break;
		case sf::Event::MouseMoved:
		{
			if (m_moving)
				MoveTo(sf::Vector2f(e->mouseMove.x, e->mouseMove.y) - m_mouseOffset);
		} break;
	default: ;
	}

	for (auto widget : m_visualInputs)
	{
		if (widget)
			widget->Event(e);
	}

	for (auto widget : m_visualOutputs)
	{
		if (widget)
			widget->Event(e);
	}
}

void VisualNode::ConnectAllWires()
{
	auto allNodes = m_pNodeMgr->GetAllNodes();

	for (auto inputWidget : m_visualInputs)
	{
		auto connectedWidget = &inputWidget->GetData()->m_connected;

		if (connectedWidget->size() == 0)
			continue;

		for (auto sharedVisualNode : *allNodes)
		{
			if (sharedVisualNode == this)
				continue;

			VisualNode *visualNode = sharedVisualNode.data();

			auto visualOutputs = visualNode->GetAllOutputs();


			for (auto sharedVisualOutput : *visualOutputs)
			{
				VisualWidget *outputWidget = sharedVisualOutput.data();

				if (outputWidget->GetData()->GetID() == connectedWidget->at(0))
				{
					inputWidget->ConnectWire();
					outputWidget->ConnectWire();

					break;
				}
			}
		}
	}

	auto idWireExecTo = m_pData->m_idExecTo;

	if (idWireExecTo)
	{
		for (auto sharedVisualNode : *allNodes)
		{
			if (sharedVisualNode == this)
				continue;

			VisualNode *visualNode = sharedVisualNode.data();

			if (visualNode->GetData()->GetID() == idWireExecTo)
			{
				Connect(ExecType::ExecTo);
				visualNode->Connect(ExecType::ExecFrom);
				break;
			}
		}
	}
}

void VisualNode::MoveTo(sf::Vector2f pos) const
{
	m_pData->m_pos = pos;

	m_pHeader->setPosition(pos);
	m_pBody->setPosition(sf::Vector2f(pos.x, pos.y + m_pHeader->getSize().y));

	if (m_pShapeExecFrom)
		m_pShapeExecFrom->setPosition(pos + sf::Vector2f(m_pShapeExecFrom->getRadius() / 2 + m_pHeader->getOutlineThickness(), m_pHeader->getSize().y / 2));

	if (m_pShapeExecTo)
		m_pShapeExecTo->setPosition(pos + sf::Vector2f(m_boxWidth - m_pShapeExecFrom->getRadius() - 1.f - m_pHeader->getOutlineThickness(), m_pHeader->getSize().y / 2));

	if (m_pWireExecFrom)
		m_pWireExecFrom->SetEndPos(m_pShapeExecFrom->getPosition());

	if (m_pWireExecTo)
		m_pWireExecTo->SetStartPos(m_pShapeExecTo->getPosition());

	m_pTitle->setPosition(m_pShapeExecFrom ? sf::Vector2f(m_pShapeExecFrom->getPosition().x + m_pShapeExecFrom->getRadius() * 2, pos.y) : pos + sf::Vector2f(8.f, 0));

	for (auto widget : m_visualInputs)
	{
		if (widget)
			widget->MoveTo(pos);
	}

	for (auto widget : m_visualOutputs)
	{
		if (widget)
			widget->MoveTo(pos);
	}
}

void VisualNode::AddExecFrom()
{
	if (!m_pShapeExecFrom)
	{
		m_pShapeExecFrom = QSharedPointer<sf::CircleShape>(new sf::CircleShape(4, 3));
		m_pShapeExecFrom->setOutlineColor(sf::Color::White);
		m_pShapeExecFrom->setOutlineThickness(2.f);
		m_pShapeExecFrom->setFillColor(sf::Color::Transparent);
		m_pShapeExecFrom->setOrigin(m_pShapeExecFrom->getRadius(), m_pShapeExecFrom->getRadius());
		m_pShapeExecFrom->setRotation(90);

		MoveTo(m_pData->m_pos);
	}
}

void VisualNode::AddExecTo()
{
	if (!m_pShapeExecTo)
	{
		m_pShapeExecTo = QSharedPointer<sf::CircleShape>(new sf::CircleShape(4, 3));
		m_pShapeExecTo->setOutlineColor(sf::Color::White);
		m_pShapeExecTo->setOutlineThickness(2.f);
		m_pShapeExecTo->setFillColor(sf::Color::Transparent);
		m_pShapeExecTo->setOrigin(m_pShapeExecTo->getRadius(), m_pShapeExecTo->getRadius());
		m_pShapeExecTo->setRotation(90);

		MoveTo(m_pData->m_pos);
	}
}

void VisualNode::ConnectedWith(VisualNode *node, ExecType execType)
{
	if (execType == ExecType::ExecFrom)
	{
		m_pData->m_idExecFrom = node->GetData()->GetID();
	}
	else if (execType == ExecType::ExecTo)
	{
		m_pData->m_idExecTo = node->GetData()->GetID();
	}
}

void VisualNode::Connect(ExecType execType)
{
	if (execType == ExecType::ExecFrom)
	{
		if (m_pWireExecFrom)
			m_pNodeMgr->GetWireMgr()->Disconnect(m_pWireExecFrom);

		m_pWireExecFrom = static_cast<WireExec*>(m_pNodeMgr->GetWireMgr()->ConnectExec(m_pShapeExecFrom->getPosition() + sf::Vector2f(0.f, 4.f), WireMgr::END, this));

		if (m_pWireExecFrom == nullptr)
			return;

		if (m_pWireExecFrom->m_pStartNode == this)
		{
			m_pNodeMgr->GetWireMgr()->Disconnect(m_pWireExecFrom);
			return;
		}

		SetPin(true, ExecType::ExecFrom);
	}
	else if (execType == ExecType::ExecTo)
	{
		if (m_pWireExecTo)
			m_pNodeMgr->GetWireMgr()->Disconnect(m_pWireExecTo);

		m_pWireExecTo = static_cast<WireExec*>(m_pNodeMgr->GetWireMgr()->ConnectExec(m_pShapeExecTo->getPosition() + sf::Vector2f(0.f, 4.f), WireMgr::START, this));

		if (m_pWireExecTo == nullptr)
			return;

		if (m_pWireExecTo->m_pEndNode == this)
		{
			m_pNodeMgr->GetWireMgr()->Disconnect(m_pWireExecTo);
			return;
		}

		SetPin(true, ExecType::ExecTo);
	}
}

void VisualNode::Disconnect(WireExec *wire)
{
	if (m_pWireExecFrom == wire)
	{
		m_pData->m_idExecFrom = 0;
		SetPin(false, ExecType::ExecFrom);
		m_pWireExecFrom = nullptr;
	}

	if (m_pWireExecTo == wire)
	{
		m_pData->m_idExecTo = 0;
		SetPin(false, ExecType::ExecTo);
		m_pWireExecTo = nullptr;
	}
}

void VisualNode::SetPin(bool pin, ExecType pinType)
{
	if (pinType == ExecType::ExecFrom)
	{
		if (pin)
			m_pShapeExecFrom->setFillColor(sf::Color::White);
		else
			m_pShapeExecFrom->setFillColor(sf::Color::Transparent);
	}
	else if (pinType == ExecType::ExecTo)
	{
		if (pin)
			m_pShapeExecTo->setFillColor(sf::Color::White);
		else
			m_pShapeExecTo->setFillColor(sf::Color::Transparent);
	}
}
