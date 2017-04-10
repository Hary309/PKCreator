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

class QEvent;

class VisualWidget;

class VisualNode
{
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

	sf::Vector2f							m_mouseOffset;

	bool									m_moving;

public:
	VisualNode(NodeMgr *parent, Node *data, sf::Color defaultColor = sf::Color(0x757575FF), int width = 200);
	~VisualNode();

	void Render(sf::RenderWindow *renderer);
	void Event(sf::Event *e);

	void MoveTo(sf::Vector2f pos) const;

	int GetWidth() const { return m_boxWidth; }

	Node *GetData() const { return m_pData; }
	NodeMgr *GetNodeMgr() const { return m_pNodeMgr; }
};

