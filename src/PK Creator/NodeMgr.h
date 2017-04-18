/*
*********************************************************************
* File          : NodeMgr.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <QSharedPointer>
#include <QVector>

namespace sf
{
	class RenderWindow;
	class Font;
	class Event;
}

class Node;
class VisualNode;
class QEvent;
class WireMgr;

class NodeMgr
{
private:
	QVector<QSharedPointer<VisualNode>>		m_visualNodes;

	QVector<QSharedPointer<Node>>			*m_pNodes;

	QSharedPointer<sf::Font>				m_pFont;

	QSharedPointer<WireMgr>					m_pWireMgr;

public:
	NodeMgr(QVector<QSharedPointer<Node>> *nodes);
	~NodeMgr();

	VisualNode *AddNode(Node *node);
	bool RemoveNode(Node *node);
	bool RemoveNode(VisualNode *visualNode);

	VisualNode *GetNode(Node *node);

	auto GetAllNodes() const { return &m_visualNodes; }

	void Render(sf::RenderWindow *renderer);
	void Event(sf::Event *e);

	sf::Font *GetFont() const { return m_pFont.data(); }
	WireMgr *GetWireMgr() { return m_pWireMgr.data(); }
};

