#include "InputWidget.h"

#include <SFML/Graphics.hpp>

#include <Widget.h>
#include <NodeMgr.h>
#include <VisualNode.h>

InputWidget::InputWidget(VisualNode *parent, Widget *data, sf::Vector2f offset)
	: VisualWidget(parent, data, offset)
{
	auto boxSize = m_pSpace->getSize();
	boxSize.x -= VisualWidget::m_verMargin;
	m_pSpace->setSize(boxSize);
}


InputWidget::~InputWidget()
{
}

void InputWidget::Render(sf::RenderWindow *renderer)
{
	renderer->draw(*m_pSpace);
	renderer->draw(*m_pName);
	renderer->draw(*m_pPin);
}

void InputWidget::Event(sf::Event * e)
{
}

void InputWidget::MoveTo(sf::Vector2f pos)
{
	m_pos = pos + m_offset;

	m_pSpace->setPosition(m_pos);

	m_pPin->setPosition(m_pos + sf::Vector2f(m_verMargin + m_pPin->getSize().x * 2, m_height / 2));

	m_pName->setPosition(sf::Vector2f(m_pPin->getPosition().x + m_pPin->getSize().x + m_verMargin, m_pos.y + m_height / 2));
}
