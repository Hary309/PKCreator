/*
*********************************************************************
* File          : Wire.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "Wire.h"

#include <SFML\Graphics.hpp>
#include <QVector>

Wire::Wire()
{
	m_wireColor = sf::Color::White;
}

Wire::~Wire()
{
}

void Wire::Render(sf::RenderWindow *renderer)
{
	auto offset = m_endPos - m_startPos;

	float len = 40.f;
	const float maxLen = 120.f;
	constexpr float minOffset = 60.f;

	if (offset.x < minOffset)
	{
		float offsetAbs = abs(offset.x - minOffset - len);

		if (offsetAbs > maxLen)
			offsetAbs = maxLen;

		len = offsetAbs;
	}

	QVector<sf::Vector2f> points;
	points.push_back(sf::Vector2f(m_startPos));
	points.push_back(sf::Vector2f(m_startPos.x + len, m_startPos.y));
	points.push_back(sf::Vector2f(m_startPos.x + len, m_startPos.y + offset.y / 2));
	points.push_back(sf::Vector2f(m_endPos.x - len, m_startPos.y + offset.y / 2));
	points.push_back(sf::Vector2f(m_endPos.x - len, m_endPos.y));
	points.push_back(sf::Vector2f(m_endPos));

	/*{
		QVector<sf::Vertex> line;

		sf::Color lineColor = sf::Color::Red;

		for (int i = 0 ; i < points.size() ; ++i)
		{
			line.push_back(sf::Vertex(points[i], lineColor));
		}

		renderer->draw(&line[0], line.size(), sf::PrimitiveType::LineStrip);
	}*/

	{
		QVector<sf::Vertex> line;

		for (int i = 0; i <= 100; i+=5)
		{
			float perc = i / 100.f;

			line.push_back(sf::Vertex(CalcBezierCurve(points, perc)[0], m_wireColor));
		
		}

		renderer->draw(&line[0], line.size(), sf::PrimitiveType::LineStrip);
	}
}

QVector<sf::Vector2f> Wire::CalcBezierCurve(QVector<sf::Vector2f>& points, float perc)
{
	QVector<sf::Vector2f> step;

	if (points.size() == 1)
		return points;

	for (int i = 0; i < points.size() - 1; ++i)
	{
		step.push_back(points[i] + (points[i + 1] - points[i]) * perc);
	}

	return CalcBezierCurve(step, perc);
}
