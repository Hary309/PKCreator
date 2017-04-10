#include "OutputWidget.h"

#include <SFML/Graphics.hpp>

#include <Widget.h>
#include <NodeMgr.h>
#include <VisualNode.h>

OutputWidget::OutputWidget(VisualNode *parent, Widget *data, sf::Vector2f offset)
	: VisualWidget(parent, data, offset)
{
}

OutputWidget::~OutputWidget()
{
}

void OutputWidget::Render(sf::RenderWindow *renderer)
{
	renderer->draw(*m_pSpace);
	renderer->draw(*m_pName);
	renderer->draw(*m_pPin);
}

void OutputWidget::Event(sf::Event *e)
{
}

void OutputWidget::MoveTo(sf::Vector2f pos)
{
	m_pos = pos + m_offset;

	m_pSpace->setPosition(m_pos);

	m_pPin->setPosition(m_pos + sf::Vector2f(m_pSpace->getSize().x - m_verMargin, m_height / 2));

	sf::FloatRect bounds = m_pName->getLocalBounds();
	m_pName->setPosition(sf::Vector2f(m_pPin->getPosition().x - m_pPin->getSize().x * 2 - bounds.width - m_verMargin, m_pos.y + m_height / 2));
}
