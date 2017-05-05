/*
*********************************************************************
* File          : Tooltip.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <SFML/System/Vector2.hpp>

#include <QSharedPointer>

namespace sf
{
	class RenderWindow;
	class RectangleShape;
	class Text;
}

class NodeMgr;

class Tooltip
{
private:
	NodeMgr									*m_pParent;

	QSharedPointer<sf::RectangleShape>		m_pBackground;
	QSharedPointer<sf::Text>				m_pText;

	bool									m_render;

	static Tooltip *s_pInst;

public:
	explicit Tooltip(NodeMgr *parent);
	~Tooltip();

	void SetText(const QString &text);
	void SetPos(const sf::Vector2f &pos);

	void Show() { m_render = true; }
	void Hide() { m_render = false; }

	void Render(sf::RenderWindow *renderer);

	static Tooltip *Get() { return s_pInst; }
};

