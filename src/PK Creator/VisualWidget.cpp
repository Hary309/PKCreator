/*
*********************************************************************
* File          : VisualWidget.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "VisualWidget.h"

#include <SFML/Graphics.hpp>

#include <Widget.h>
#include <NodeMgr.h>
#include <VisualNode.h>

VisualWidget::VisualWidget(VisualNode *parent, Widget *data, sf::Vector2f offset)
	: m_pParent(parent), m_pData(data), m_offset(offset)
{
	m_height = 20.f;

	m_pSpace = QSharedPointer<sf::RectangleShape>(new sf::RectangleShape(sf::Vector2f(parent->GetWidth() / 2 - m_verMargin, m_height)));
	m_pSpace->setFillColor(sf::Color(0x505050FF));

	m_pName = QSharedPointer<sf::Text>(new sf::Text());
	m_pName->setFont(*parent->GetNodeMgr()->GetFont());
	m_pName->setCharacterSize(16);
	m_pName->setString(m_pData->GetName().toStdString());
	m_pName->setOrigin(0.f, m_pName->getCharacterSize() / 1.5f);

	m_pPin = QSharedPointer<sf::RectangleShape>(new sf::RectangleShape(sf::Vector2f(8.f, 8.f)));
	m_pPin->setOutlineColor(m_pData->m_color);
	m_pPin->setOutlineThickness(1.f);
	m_pPin->setFillColor(sf::Color::Transparent);
	m_pPin->setOrigin(m_pPin->getSize().x, m_pPin->getSize().y / 2);
}


VisualWidget::~VisualWidget()
{
}

void VisualWidget::SetPin(bool pin) const
{
	if (pin)
		m_pPin->setFillColor(m_pPin->getOutlineColor());
	else
		m_pPin->setFillColor(sf::Color::Transparent);
}
