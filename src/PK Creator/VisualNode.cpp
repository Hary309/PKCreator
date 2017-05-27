/*
*********************************************************************
* File          : VisualNode.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "VisualNode.h"

#include <QScreen>
#include <QMessageBox>

#include <BlueprintEditor.h>

#include <NodeMgr.h>
#include <Node.h>

#include <WireMgr.h>
#include <Wire.h>
#include <WireExec.h>

#include <Widget.h>
#include <InputDataWidget.h>
#include <OutputDataWidget.h>
#include <OutputExecWidget.h>

#include <SFML/Graphics.hpp>

VisualNode::VisualNode(NodeMgr *nodeMgr, Node *data, sf::Color defaultColor, int width)
	: m_pNodeMgr(nodeMgr), m_pData(data), m_boxWidth(width), m_defaultColor(defaultColor), m_pFont(m_pNodeMgr->GetFont())
{
	if (m_pData->m_type == Node::FUNCTION)
		m_defaultColor = sf::Color(0x388E3CAA);
	else if (m_pData->m_type == Node::EVENT)
		m_defaultColor = sf::Color(0x0288D1AA);
	else if (m_pData->m_type == Node::VARIABLE)
		m_defaultColor = sf::Color(0xD32F2FAA);
	else if (m_pData->m_type == Node::INLINE_VARIABLE)
		m_defaultColor = sf::Color(0xE65100AA);
	else if (m_pData->m_type == Node::CONDITION)
		m_defaultColor = sf::Color(0x6D4C41AA);
	
	m_pHeader = QSharedPointer<sf::RectangleShape>(new sf::RectangleShape(sf::Vector2f(m_boxWidth, 20)));
	m_pBody = QSharedPointer<sf::RectangleShape>(new sf::RectangleShape(sf::Vector2f(m_boxWidth, 0)));

	m_pHeader->setFillColor(m_defaultColor);
	m_pBody->setFillColor(sf::Color(0x424242AA));

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
			auto offset = sf::Vector2f(VisualWidget::m_horMargin, i == 0 ? 20.f + VisualWidget::m_verMargin : 20.f + (i + 1) * VisualWidget::m_verMargin + i * m_visualInputs.last()->GetHeight());

			auto inputWidget = QSharedPointer<InputDataWidget>(new InputDataWidget(this, widget.data(), offset));

			m_visualInputs.push_back(inputWidget);

			auto bodySize = m_pBody->getSize();

			if (bodySize.y < offset.y)
				m_pBody->setSize(sf::Vector2f(bodySize.x, offset.y + VisualWidget::m_verMargin));
		}
	}

	for (int i = 0; i < m_pData->m_outputs.size(); ++i)
	{
		auto widget = m_pData->m_outputs[i];

		if (widget)
		{
			if (widget->m_type == Widget::DATA)
			{
				auto offset = sf::Vector2f(m_boxWidth / 2, i == 0 ? 20.f + VisualWidget::m_verMargin : 20.f + (i + 1) * VisualWidget::m_verMargin + i * m_visualOutputs.last()->GetHeight());

				auto outputWidget = QSharedPointer<OutputDataWidget>(new OutputDataWidget(this, widget.data(), offset));

				m_visualOutputs.push_back(outputWidget);

				auto bodySize = m_pBody->getSize();

				if (bodySize.y < offset.y)
					m_pBody->setSize(sf::Vector2f(bodySize.x, offset.y + VisualWidget::m_verMargin));
			}
			else if (widget->m_type == Widget::EXEC)
			{
				auto offset = sf::Vector2f(m_boxWidth / 2, i == 0 ? 20.f + VisualWidget::m_verMargin : 20.f + (i + 1) * VisualWidget::m_verMargin + i * m_visualOutputs.last()->GetHeight());

				auto outputWidget = QSharedPointer<OutputExecWidget>(new OutputExecWidget(this, widget.data(), offset));

				m_visualOutputs.push_back(outputWidget);

				auto bodySize = m_pBody->getSize();

				if (bodySize.y < offset.y)
					m_pBody->setSize(sf::Vector2f(bodySize.x, offset.y + VisualWidget::m_verMargin));
			}
		}
	}

	m_pWireExecTo = nullptr;
	m_pWireExecFrom = nullptr;

	if (m_pData->m_execFrom)
		AddExecFrom();

	if (m_pData->m_execTo)
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
	if (!m_pNodeMgr)
		return;

	if (!m_pNodeMgr->GetBpEditor())
		return;

	sf::Vector2f viewOffset = m_pNodeMgr->GetBpEditor()->GetViewOffset();
	float scale = m_pNodeMgr->GetBpEditor()->GetScale();

	switch (e->type)
	{
		case sf::Event::MouseButtonPressed:
		{
			if (e->mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2f nodePos = m_pData->m_pos;
				sf::Vector2f size = m_pHeader->getSize();
				sf::Vector2f mousePos = sf::Vector2f(e->mouseButton.x, e->mouseButton.y) * scale - viewOffset;

				if (mousePos.x > nodePos.x &&
					mousePos.x < nodePos.x + size.x &&
					mousePos.y > nodePos.y &&
					mousePos.y < nodePos.y + size.y)
				{
					if (mousePos.x < nodePos.x + 14.f && m_pData->m_execFrom)
					{
						Connect(ExecType::ExecFrom);
					}
					else if (mousePos.x > nodePos.x + m_boxWidth - 14.f && m_pData->m_execTo)
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
				MoveTo(sf::Vector2f(e->mouseMove.x, e->mouseMove.y) * scale - m_mouseOffset - viewOffset);
		} break;
		case sf::Event::KeyReleased:
		{
			if (e->key.code == Qt::Key_Delete)
			{
				auto pos = sf::Mouse::getPosition(*m_pNodeMgr->GetBpEditor()->GetRenderer());

				sf::Vector2f nodePos = m_pData->m_pos;
				sf::Vector2f size = m_pHeader->getSize();
				sf::Vector2f mousePos = sf::Vector2f(pos.x, pos.y) * scale - viewOffset;

				if (mousePos.x > nodePos.x &&
					mousePos.x < nodePos.x + size.x &&
					mousePos.y > nodePos.y &&
					mousePos.y < nodePos.y + size.y)
				{
					if (!m_pData->m_type == Node::EVENT)
					{
						auto result = QMessageBox::question(m_pNodeMgr->GetBpEditor(), "PK Creator", "Are your sure you want to remove " + m_pData->GetName() + " node?");

						if (result == QMessageBox::Yes)
						{
							m_pNodeMgr->RemoveNode(m_pData);
						}
					}
				}
			}
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

void VisualNode::MoveTo(sf::Vector2f pos) const
{
	m_pData->m_pos = pos;

	m_pHeader->setPosition(pos);
	m_pBody->setPosition(sf::Vector2f(pos.x, pos.y + m_pHeader->getSize().y));

	if (m_pShapeExecFrom)
		m_pShapeExecFrom->setPosition(pos + sf::Vector2f(m_pShapeExecFrom->getRadius() / 2 + m_pHeader->getOutlineThickness(), m_pHeader->getSize().y / 2));

	if (m_pShapeExecTo)
		m_pShapeExecTo->setPosition(pos + sf::Vector2f(m_boxWidth - m_pShapeExecTo->getRadius() - 1.f - m_pHeader->getOutlineThickness(), m_pHeader->getSize().y / 2));

	if (m_pWireExecFrom)
		m_pWireExecFrom->SetEndPos(m_pShapeExecFrom->getPosition());

	if (m_pWireExecTo)
		m_pWireExecTo->SetStartPos(m_pShapeExecTo->getPosition());

	static const float margin = 8.f;

	m_pTitle->setPosition(m_pShapeExecFrom ? sf::Vector2f(m_pShapeExecFrom->getPosition().x + m_pShapeExecFrom->getRadius() * 2 + margin, pos.y) : pos + sf::Vector2f(margin, 0));

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

void VisualNode::ConnectAllWires()
{
	printf("\n\n");

	auto allNodes = m_pNodeMgr->GetAllNodes();

	for (auto inputWidget : m_visualInputs)
	{
		auto connectedWidget = &inputWidget->GetData()->m_connected;

		if (!connectedWidget)
			continue;

		if (connectedWidget->isEmpty())
			continue;

		for (auto sharedVisualNode : *allNodes)
		{
			if (sharedVisualNode == this)
				continue;

			auto visualOutputs = sharedVisualNode->GetAllOutputs();

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

	if (m_pData->m_type == Node::CONDITION)
	{
		for (auto outputWidget : m_visualOutputs)
		{
			auto connectedNode = &outputWidget->GetData()->m_connected;

			if (!connectedNode)
				continue;

			if (connectedNode->isEmpty())
				continue;

			for (auto sharedVisualNode : *allNodes)
			{
				if (sharedVisualNode == this)
					continue;

				if (connectedNode->isEmpty())
					continue;

				if (sharedVisualNode->GetData()->GetID() == connectedNode->first())
				{
					outputWidget->ConnectWire();
					sharedVisualNode->Connect(ExecType::ExecFrom);
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

			if (sharedVisualNode->GetData()->GetID() == idWireExecTo)
			{
				Connect(ExecType::ExecTo);
				sharedVisualNode->Connect(ExecType::ExecFrom);
				break;
			}
		}
	}
}

void VisualNode::DisconnectAll()
{
	auto wireMgr = m_pNodeMgr->GetWireMgr();

	wireMgr->Disconnect(m_pWireExecFrom);
	wireMgr->Disconnect(m_pWireExecTo);

	for (auto visualWidget : m_visualInputs)
	{
		InputDataWidget *inputWidget = static_cast<InputDataWidget*>(visualWidget.data());

		wireMgr->Disconnect(reinterpret_cast<Wire*>(inputWidget->GetWire()));
	}

	for (auto visualWidget : m_visualOutputs)
	{
		if (m_pData->m_type == Node::CONDITION)
		{
			OutputExecWidget *outputWidget = static_cast<OutputExecWidget*>(visualWidget.data());

			WireExec *wire = outputWidget->GetWire();

			wireMgr->Disconnect(reinterpret_cast<Wire*>(wire));

		}
		else
		{
			OutputDataWidget *outputWidget = static_cast<OutputDataWidget*>(visualWidget.data());

			// Not optimal but the easiest way
			auto wires = *outputWidget->GetWires();

			for (auto wire : wires)
			{
				wireMgr->Disconnect(reinterpret_cast<Wire*>(wire));
			}
		}
	}
}

void VisualNode::ConnectedWith(VisualNode *node, ExecType execType) const
{
	if (m_pData->m_type == Node::CONDITION)
	{
		bool connected = false;

		for (auto widget : m_visualOutputs)
		{
			OutputExecWidget *outputExec = static_cast<OutputExecWidget*>(widget.data());
			connected = outputExec->ConnectedWith(node);
			
			if (connected)
				break;
		}

		if (!connected)
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
	}
	else
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

		MoveTo(m_pData->m_pos);
	}
	else if (execType == ExecType::ExecTo)
	{
		if (m_pWireExecTo)
			m_pNodeMgr->GetWireMgr()->Disconnect(m_pWireExecTo);

		if (!m_pShapeExecTo)
			return;

		m_pWireExecTo = static_cast<WireExec*>(m_pNodeMgr->GetWireMgr()->ConnectExec(m_pShapeExecTo->getPosition() + sf::Vector2f(0.f, 4.f), WireMgr::START, this));

		if (m_pWireExecTo == nullptr)
			return;

		if (m_pWireExecTo->m_pEndNode == this)
		{
			m_pNodeMgr->GetWireMgr()->Disconnect(m_pWireExecTo);
			return;
		}

		SetPin(true, ExecType::ExecTo);

		MoveTo(m_pData->m_pos);
	}
}

void VisualNode::Disconnect(WireExec *wire)
{
	if (m_pData->m_type == Node::CONDITION)
	{
		for (auto widget : m_visualOutputs)
		{
			OutputExecWidget *outputExec = static_cast<OutputExecWidget*>(widget.data());

			outputExec->Disconnect(wire);
		}
	}

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

void VisualNode::SetPin(bool pin, ExecType pinType) const
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
