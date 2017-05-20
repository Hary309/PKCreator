/*
*********************************************************************
* File          : VisualNode.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <QSharedPointer>
#include <QVector>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf
{
	class RenderWindow;
	class Event;
	class Font;
	class Text;
	class RectangleShape;
	class CircleShape;
}

class NodeMgr;
class Node;

class WireExec;

class QEvent;

class VisualWidget;

class VisualNode
{
public:
	enum ExecType
	{
		ExecFrom,	// left
		ExecTo		// right
	};

private:
	NodeMgr									*m_pNodeMgr;

	Node									*m_pData;

	QVector<QSharedPointer<VisualWidget>>	m_visualInputs;
	QVector<QSharedPointer<VisualWidget>>	m_visualOutputs;

	int										m_boxWidth;
	sf::Color								m_defaultColor;

	sf::Font								*m_pFont;

	QSharedPointer<sf::RectangleShape>		m_pHeader;
	QSharedPointer<sf::RectangleShape>		m_pBody;
	QSharedPointer<sf::Text>				m_pTitle;

	QSharedPointer<sf::CircleShape>			m_pShapeExecFrom;
	QSharedPointer<sf::CircleShape>			m_pShapeExecTo;

	WireExec								*m_pWireExecFrom;
	WireExec								*m_pWireExecTo;

	sf::Vector2f							m_mouseOffset;

	bool									m_moving;

public:
	VisualNode(NodeMgr *parent, Node *data, sf::Color defaultColor = sf::Color(0x757575FF), int width = 200);
	~VisualNode();

	void Render(sf::RenderWindow *renderer);
	void Event(sf::Event *e);

	void MoveTo(sf::Vector2f pos) const;

	void AddExecFrom();
	void AddExecTo();

	void ConnectAllWires();
	void DisconnectAll();
	void ConnectedWith(VisualNode *node, ExecType execType) const;
	void Connect(ExecType execType);
	void Disconnect(WireExec *wire);

	void SetPin(bool pin, ExecType pinType) const;

	int GetWidth() const { return m_boxWidth; }

	auto GetAllInputs() const { return &m_visualInputs; }
	auto GetAllOutputs() const { return &m_visualOutputs; }

	Node *GetData() const { return m_pData; }
	NodeMgr *GetNodeMgr() const { return m_pNodeMgr; }
};

