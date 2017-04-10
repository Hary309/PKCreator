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

class NodeMgr
{
private:
	QVector<QSharedPointer<VisualNode>>		m_visualNodes;

	QVector<QSharedPointer<Node>>			*m_pNodes;

	QSharedPointer<sf::Font>				m_pFont;

public:
	NodeMgr(QVector<QSharedPointer<Node>> *nodes);
	~NodeMgr();

	VisualNode *AddNode(Node *node);
	bool RemoveNode(Node *node);
	bool RemoveNode(VisualNode *visualNode);

	void Render(sf::RenderWindow *renderer);
	void Event(sf::Event *e);

	sf::Font *GetFont() const { return m_pFont.data(); }
};

