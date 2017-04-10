#include "NodeMgr.h"

#include <Node.h>
#include <VisualNode.h>

#include <SFML/Graphics/Font.hpp>

NodeMgr::NodeMgr(QVector<QSharedPointer<Node>> *nodes)
	: m_pNodes(nodes)
{
	m_pFont = QSharedPointer<sf::Font>(new sf::Font());
	m_pFont->loadFromFile("calibri.ttf");

	for (auto node : *m_pNodes)
	{
		auto sharedVisualNode = QSharedPointer<VisualNode>(new VisualNode(this, node.data()));
		m_visualNodes.push_back(sharedVisualNode);
	}
}

NodeMgr::~NodeMgr()
{
}

VisualNode *NodeMgr::AddNode(Node *node)
{
	//if (!m_pNodes->empty())
	//	return nullptr;

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

void NodeMgr::Render(sf::RenderWindow *renderer)
{
	for (auto node : m_visualNodes)
	{
		if (node)
			node->Render(renderer);
	}
}

void NodeMgr::Event(sf::Event *e)
{
	for (auto node : m_visualNodes)
	{
		if (node)
			node->Event(e);
	}
}
