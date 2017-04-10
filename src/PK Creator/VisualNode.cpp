#include "VisualNode.h"

#include <NodeMgr.h>
#include <Node.h>

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
			auto offset = sf::Vector2f(VisualWidget::m_verMargin, i == 0 ? 20.f + VisualWidget::m_horMargin : 20.f + (i + 1) * VisualWidget::m_horMargin + i * m_visualOutputs.last()->GetHeight());

			auto outputWidget = QSharedPointer<InputWidget>(new InputWidget(this, widget.data(), offset));

			m_visualOutputs.push_back(outputWidget);

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
					m_moving = true;

					m_mouseOffset = mousePos - nodePos;

					MoveTo(mousePos - m_mouseOffset);
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
}

void VisualNode::MoveTo(sf::Vector2f pos) const
{
	m_pData->m_pos = pos;

	m_pHeader->setPosition(pos);
	m_pBody->setPosition(sf::Vector2f(pos.x, pos.y + m_pHeader->getSize().y));
	m_pTitle->setPosition(pos + sf::Vector2f(8.f, 0));

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
