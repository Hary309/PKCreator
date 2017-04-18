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

#include <SFML/Graphics/Font.hpp>

NodeMgr::NodeMgr(QVector<QSharedPointer<Node>> *nodes)
	: m_pNodes(nodes)
{
	m_pFont = QSharedPointer<sf::Font>(new sf::Font());
	m_pFont->loadFromFile("calibri.ttf");

	m_pWireMgr = QSharedPointer<WireMgr>(new WireMgr(this));

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
	if (m_pNodes->size() == 2)
		return nullptr;

	if (!node)
		return nullptr;

	auto sharedVisualNode = QSharedPointer<VisualNode>(new VisualNode(this, node));
	m_visualNodes.push_back(sharedVisualNode);

	auto sharedNode = QSharedPointer<Node>(node);
	m_pNodes->push_back(sharedNode);

	return sharedVisualNode.data();
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
