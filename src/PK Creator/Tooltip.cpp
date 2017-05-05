/*
*********************************************************************
* File          : Tooltip.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "Tooltip.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

#include <NodeMgr.h>
#include <BlueprintEditor.h>

Tooltip *Tooltip::s_pInst = nullptr;

Tooltip::Tooltip(NodeMgr *parent)
	: m_pParent(parent)
{
	s_pInst = this;

	m_pBackground = QSharedPointer<sf::RectangleShape>(new sf::RectangleShape(sf::Vector2f()));

	m_pBackground->setFillColor(sf::Color(0x757575FF));
	m_pBackground->setOutlineColor(sf::Color(0xE0E0E0FF));
	m_pBackground->setOutlineThickness(1.f);

	m_pText = QSharedPointer<sf::Text>(new sf::Text());
	m_pText->setFont(*parent->GetFont());
	m_pText->setCharacterSize(14);
	m_pText->setOrigin(0.f, 14 / 2);
}

Tooltip::~Tooltip()
{

}

void Tooltip::SetText(const QString &text)
{
	m_pText->setString(text.toStdString());
}

void Tooltip::SetPos(const sf::Vector2f &pos)
{
	sf::Vector2f viewOffset = m_pParent->GetBpEditor()->GetViewOffset();
	float scale = m_pParent->GetBpEditor()->GetScale();

	sf::Vector2f newPos = sf::Vector2f(pos.x, pos.y) * scale - viewOffset;

	sf::Vector2f m_pos = newPos + sf::Vector2f(0.f, 16.f);

	m_pBackground->setPosition(m_pos);
	m_pText->setPosition(m_pos + sf::Vector2f(4.f, m_pText->getCharacterSize() / 2));

	sf::FloatRect bounds = m_pText->getLocalBounds();

	m_pBackground->setSize(sf::Vector2f(bounds.width + 4.f * 2, m_pText->getCharacterSize() * 1.4));
}

void Tooltip::Render(sf::RenderWindow *renderer)
{
	if (m_render)
	{
		if (m_pBackground)
			renderer->draw(*m_pBackground);

		if (m_pText)
			renderer->draw(*m_pText);
	}
}
