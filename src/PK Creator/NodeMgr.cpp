/*
*********************************************************************
* File          : NodeMgr.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "NodeMgr.h"

#include <conio.h>

#include <Node.h>
#include <VisualNode.h>
#include <WireMgr.h>
#include <Tooltip.h>
#include <Widget.h>

#include <SFML/Graphics/Font.hpp>
#include <QCursor>

NodeMgr::NodeMgr(BlueprintEditor *parent, QVector<QSharedPointer<Node>> *nodes)
	: m_pParent(parent), m_pNodes(nodes)
{
	m_pFont = QSharedPointer<sf::Font>(new sf::Font());
	m_pFont->loadFromFile("calibri.ttf");

	m_pWireMgr = QSharedPointer<WireMgr>(new WireMgr(this));

	m_pTooltip = QSharedPointer<Tooltip>(new Tooltip(this));

	for (auto node : *m_pNodes)
	{
		if (node)
		{
			auto sharedVisualNode = QSharedPointer<VisualNode>(new VisualNode(this, node.data()));
			m_visualNodes.push_back(sharedVisualNode);
		}
	}

	for (auto visualNode : m_visualNodes)
	{
		if (visualNode)
			visualNode->ConnectAllWires();
	}
}

NodeMgr::~NodeMgr()
{
}

VisualNode *NodeMgr::AddNode(Node *node)
{
	if (!node)
		return nullptr;

	auto sharedVisualNode = QSharedPointer<VisualNode>(new VisualNode(this, node));
	m_visualNodes.push_back(sharedVisualNode);

	auto sharedNode = QSharedPointer<Node>(node);
	m_pNodes->push_back(sharedNode);

	return sharedVisualNode.data();
}

VisualNode *NodeMgr::AddNodeFromFunctionDef(FunctionDefsMgr::FunctionDef *nodeDef, const sf::Vector2f &pos)
{
	if (!nodeDef)
		return nullptr;

	if (nodeDef->name == "")
		return nullptr;

	QPoint cursorPos = QCursor::pos();

	Node *node = new Node(nodeDef->name, pos, Node::FUNCTION);

	for (auto arg : nodeDef->args)
	{
		node->AddWidget(new Widget(node, arg.name, Widget::INPUT, arg.type));
	}

	if (nodeDef->returnValue.type != -1)
	{
		node->AddWidget(new Widget(node, nodeDef->returnValue.name, Widget::OUTPUT, nodeDef->returnValue.type));
	}	

	auto visualNode = AddNode(node);

	return visualNode;
}

bool NodeMgr::RemoveNode(Node *node)
{
	for (int i = 0; i < m_pNodes->size(); ++i)
	{
		if (m_pNodes->at(i) == node)
		{
			for (int j = 0 ; j < m_visualNodes.size(); ++j)
			{
				if (m_visualNodes[j]->GetData() == m_pNodes->at(i))
				{
					m_visualNodes[j]->DisconnectAll();
					m_visualNodes.removeAt(j);
					break;
				}
			}

			m_pNodes->removeAt(i);

			return true;
		}
	}

	return false;
}

bool NodeMgr::RemoveNode(VisualNode *visualNode)
{
	for (int i = 0; i < m_visualNodes.size(); ++i)
	{
		if (m_visualNodes[i] == visualNode)
		{
			for (int j = 0; j < m_pNodes->size(); ++j)
			{
				if (m_pNodes->at(j) == m_visualNodes[i]->GetData())
				{
					m_pNodes->removeAt(j);
					break;
				}
			}

			m_visualNodes.removeAt(i);

			return true;
		}
	}

	return false;
}

VisualNode *NodeMgr::GetNode(Node *node)
{
	for (auto visualNode : m_visualNodes)
	{
		if (visualNode)
		{
			if (visualNode->GetData() == node)
				return visualNode.data();
		}
	}
	
	return nullptr;
}

void NodeMgr::Render(sf::RenderWindow *renderer)
{
	for (auto node : m_visualNodes)
	{
		if (node)
			node->Render(renderer);
	}

	if (m_pTooltip)
		m_pTooltip->Render(renderer);

	m_pWireMgr->Render(renderer);
}

void NodeMgr::Event(sf::Event *e)
{
	for (auto node : m_visualNodes)
	{
		if (node)
			node->Event(e);
	}

	m_pWireMgr->Event(e);
}
