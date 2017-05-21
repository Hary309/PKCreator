/*
*********************************************************************
* File          : NodeMgr.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "NodeMgr.h"

#include <conio.h>

#include <BlueprintEditor.h>
#include <EventObjectItem.h>
#include <ObjectItem.h>
#include <Node.h>
#include <VisualNode.h>
#include <WireMgr.h>
#include <Tooltip.h>
#include <Widget.h>
#include <Var.h>

#include <SFML/Graphics/Font.hpp>

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
			if (node->GetType() == Node::VARIABLE)
			{
				auto vars = parent->GetData()->GetParent()->GetVars();

				for (auto var : *vars)
				{
					if (node->GetID() == var->m_id)
					{
						node->m_name = var->m_name;

						if (node->m_inputs.first()->m_dataType != var->m_type ||
							node->m_outputs.first()->m_dataType != var->m_type)
						{
							// reset connections
							node->m_inputs.first()->m_connected.clear();
							node->m_outputs.first()->m_connected.clear();
							
							// change type and color
							node->m_inputs.first()->SetDataType(var->m_type);
							node->m_outputs.first()->SetDataType(var->m_type);
						}
					}
				}
			}

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

	Node *node = new Node(nodeDef->name, pos, Node::FUNCTION);

	for (auto arg : nodeDef->args)
	{
		node->AddWidget(new Widget(node, arg.name, Widget::DATA, Widget::INPUT, arg.type));
	}

	if (nodeDef->returnValue.type != -1)
	{
		node->AddWidget(new Widget(node, nodeDef->returnValue.name, Widget::DATA, Widget::OUTPUT, nodeDef->returnValue.type));
	}	

	auto visualNode = AddNode(node);

	return visualNode;
}

VisualNode *NodeMgr::AddNodeFromVar(Var *var, const sf::Vector2f &pos)
{
	if (!var)
		return nullptr;

	if (var->m_name == "")
		return nullptr;

	if (var->m_id < 0)
		return nullptr;

	Node *node = new Node(var->m_id, var->m_name, pos, Node::VARIABLE);

	node->AddWidget(new Widget(node, "Set", Widget::DATA, Widget::INPUT, var->m_type));
	node->AddWidget(new Widget(node, "Get", Widget::DATA, Widget::OUTPUT, var->m_type));

	return AddNode(node);
}

VisualNode *NodeMgr::AddInlineVarNode(NodesWindow::InlineVarNodeItem *inlineVarNode, const sf::Vector2f &pos)
{
	Node *node = new Node(inlineVarNode->name, pos, Node::INLINE_VARIABLE);
	node->m_defaultValue = inlineVarNode->value;

	node->AddWidget(new Widget(node, "Value", Widget::DATA, Widget::OUTPUT, inlineVarNode->type));

	return AddNode(node);
}

VisualNode *NodeMgr::AddConditionNode(const NodesWindow::ConditionItem *conditionItem, const sf::Vector2f &pos)
{
	Node *node = new Node(conditionItem->name, pos, Node::CONDITION);
	node->m_defaultValue = conditionItem->conditionType;

	printf("ConditionType: %s\n", conditionItem->conditionType.toStdString().c_str());

	if (conditionItem->nInputs == 1)
	{
		node->AddWidget(new Widget(node, "a", Widget::DATA, Widget::INPUT, conditionItem->dataType));
	}
	else if (conditionItem->nInputs == 2)
	{
		node->AddWidget(new Widget(node, "a", Widget::DATA, Widget::INPUT, conditionItem->dataType));
		node->AddWidget(new Widget(node, "b", Widget::DATA, Widget::INPUT, conditionItem->dataType));
	}

	node->AddWidget(new Widget(node, "true", Widget::EXEC, Widget::OUTPUT, DataType::DATA_INTEGER));
	node->AddWidget(new Widget(node, "false", Widget::EXEC, Widget::OUTPUT, DataType::DATA_INTEGER));

	return AddNode(node);
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
