/*
*********************************************************************
* File          : NodeMgr.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <FunctionDefsMgr.h>
#include <SFML/System/Vector2.hpp>
#include <NodesWindow.h>

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
class BlueprintEditor;
class Tooltip;
class Var;


class NodeMgr
{
private:
	BlueprintEditor							*m_pParent;

	QVector<QSharedPointer<VisualNode>>		m_visualNodes;

	QVector<QSharedPointer<Node>>			*m_pNodes;

	QSharedPointer<sf::Font>				m_pFont;

	QSharedPointer<WireMgr>					m_pWireMgr;

	QSharedPointer<Tooltip>					m_pTooltip;

public:
	NodeMgr(BlueprintEditor *parent, QVector<QSharedPointer<Node>> *nodes);
	~NodeMgr();

	VisualNode *AddNode(Node *node);

	VisualNode *AddNodeFromFunctionDef(FunctionDefsMgr::FunctionDef *nodeDef, const sf::Vector2f &pos);
	VisualNode *AddNodeFromVar(Var *var, const sf::Vector2f &pos);
	VisualNode *AddInlineVarNode(NodesWindow::InlineVarNodeItem *inlineVarNode, const sf::Vector2f &pos);
	VisualNode *AddConditionNode(const NodesWindow::ConditionItem *conditionItem, const sf::Vector2f &pos);

	bool RemoveNode(Node *node);
	bool RemoveNode(VisualNode *visualNode);

	VisualNode *GetNode(Node *node);

	auto GetAllNodes() const { return &m_visualNodes; }

	void Render(sf::RenderWindow *renderer);
	void Event(sf::Event *e);

	sf::Font *GetFont() const { return m_pFont.data(); }
	WireMgr *GetWireMgr() const { return m_pWireMgr.data(); }
	BlueprintEditor *GetBpEditor() const { return m_pParent; }
};

