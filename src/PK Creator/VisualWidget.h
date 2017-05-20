/*
*********************************************************************
* File          : VisualWidget.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <SFML/System/Vector2.hpp>
#include <QSharedPointer>
#include <QTime>

namespace sf
{
	class RenderWindow;
	class Event;
	class Font;
	class Text;
	class RectangleShape;
	class CircleShape;
}

class Widget;
class VisualNode;
class Wire;
class WireData;

class VisualWidget
{
protected:
	VisualNode							*m_pParent;
	
	Widget								*m_pData;

	QSharedPointer<sf::RectangleShape>	m_pSpace;
	QSharedPointer<sf::Text>			m_pName;
	QSharedPointer<sf::CircleShape>		m_pPin;

	sf::Vector2f						m_pos;
	sf::Vector2f						m_offset;

	QTime								m_lastMouseEvent;

	bool								m_shownTooltip;

	float								m_height;

public:
	static const int m_horMargin = 4;
	static const int m_verMargin = 4;

public:
	VisualWidget(VisualNode *parent, Widget *data, sf::Vector2f offset);
	virtual ~VisualWidget();

	float GetHeight() const { return m_height; }

	virtual void ConnectedWith(VisualWidget *widget) = 0;

	virtual Wire *ConnectWire() = 0;
	virtual void Disconnect(Wire *wire) = 0;

	virtual void Render(sf::RenderWindow *renderer) = 0;
	virtual void Event(sf::Event *e);

	virtual void MoveTo(sf::Vector2f pos) = 0;

	void SetPin(bool pin) const;

	Widget *GetData() const { return m_pData; }
	VisualNode *GetParent() const { return m_pParent; }
};

