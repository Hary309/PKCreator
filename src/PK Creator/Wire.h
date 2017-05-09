/*
*********************************************************************
* File          : Wire.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

#include <QVector>

namespace sf
{
	class RenderWindow;
}

class WireMgr;

class Wire
{
public:
	enum WireType
	{
		DATA,
		EXEC
	};

protected:
	sf::Vector2f		m_startPos;
	sf::Vector2f		m_endPos;

	sf::Color			m_wireColor;

	WireType			m_wireType;

public:
	Wire();
	virtual ~Wire();

	void Render(sf::RenderWindow *renderer);

	void SetStartPos(sf::Vector2f startPos) { m_startPos = startPos; }
	void SetEndPos(sf::Vector2f endPos) { m_endPos = endPos; }
	void SetWireColor(sf::Color color) { m_wireColor = color; }

	// @return: QVector with one point
	static QVector<sf::Vector2f> CalcBezierCurve(QVector<sf::Vector2f> &points, float perc);

	virtual void Connected() = 0;
	virtual void DisconnectAll() = 0;

	WireType GetType() const { return m_wireType; }
};

