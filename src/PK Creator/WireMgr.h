/*
*********************************************************************
* File          : WireMgr.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <QSharedPointer>
#include <QVector>

#include <SFML/System/Vector2.hpp>

namespace sf
{
	class RenderWindow;
	class Event;
}

class NodeMgr;
class Wire;
class WireData;
class WireExec;
class VisualWidget;
class VisualNode;

class WireMgr
{
public:
	enum ConnectTo
	{
		START,
		END
	};

private:
	NodeMgr								*m_pNodeMgr;

	QVector<QSharedPointer<WireData>>	m_dataWires;
	QVector<QSharedPointer<WireExec>>	m_execWires;

	sf::Vector2f						m_cursorPos;

	ConnectTo							m_type;
	Wire								*m_pCurrentLine;

public:
	WireMgr(NodeMgr *parent);
	~WireMgr() = default;

	WireData *ConnectData(sf::Vector2f startPos, ConnectTo type, VisualWidget *widget);
	WireExec *ConnectExec(sf::Vector2f startPos, ConnectTo type, VisualNode *node);
	void Disconnect(Wire *wire);

	void Render(sf::RenderWindow *renderer);
	void Event(sf::Event *e);

	bool IsWire() const { return m_pCurrentLine; }
};

