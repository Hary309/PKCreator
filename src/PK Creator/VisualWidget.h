#pragma once

#include <SFML/System/Vector2.hpp>
#include <QSharedPointer>

namespace sf
{
	class RenderWindow;
	class Event;
	class Font;
	class Text;
	class RectangleShape;
}

class Widget;
class VisualNode;

class VisualWidget
{
protected:
	VisualNode			*m_pParent;
	
	Widget				*m_pData;

	QSharedPointer<sf::RectangleShape>	m_pSpace;
	QSharedPointer<sf::Text>			m_pName;
	QSharedPointer<sf::RectangleShape>	m_pPin;

	sf::Vector2f		m_pos;
	sf::Vector2f		m_offset;

	float				m_height;

public:
	static const int m_horMargin = 4;
	static const int m_verMargin = 4;

public:
	VisualWidget(VisualNode *parent, Widget *data, sf::Vector2f offset);
	virtual ~VisualWidget();

	float GetHeight() const { return m_height; }

	virtual void Render(sf::RenderWindow *renderer) = 0;
	virtual void Event(sf::Event *e) = 0;

	virtual void MoveTo(sf::Vector2f pos) = 0;
};

