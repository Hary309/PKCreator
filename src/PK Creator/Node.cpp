/*
*********************************************************************
* File          : Node.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "Node.h"

#include <QDataStream>

#include <Widget.h>

#include <Common.h>


Node::Node(const QString &name, sf::Vector2f pos, Type type)
	: m_name(name), m_pos(pos), m_type(type)
{
	m_idExecFrom = m_idExecTo = 0;

	m_id = qint64(this);

	if (type == FUNCTION)
	{
		m_execFrom = true;
		m_execTo = true;
	}
	else if (type == EVENT)
	{
		m_execFrom = false;
		m_execTo = true;
	}
	else if (type == VARIABLE)
	{
		m_execFrom = false;
		m_execTo = false;
	}
}

Node::~Node()
{
}

Widget *Node::AddWidget(Widget *widget)
{
	if (!widget)
		return nullptr;

	if (widget->GetConnType() == Widget::INPUT)
	{
		auto sharedWidget = QSharedPointer<Widget>(widget);
		m_inputs.push_back(sharedWidget);
	
		return widget;
	}
	else if (widget->GetConnType() == Widget::OUTPUT)
	{
		auto sharedWidget = QSharedPointer<Widget>(widget);
		m_outputs.push_back(sharedWidget);

		return widget;
	}

	return nullptr;
}

void Node::Load(QDataStream *const dataStream)
{
	float x, y;

	int nInputs = 0, nOutputs = 0;

	int type = 0;

	*dataStream >> m_id >> x >> y >> m_name >> type >> m_execFrom >> m_idExecFrom >> m_execTo >> m_idExecTo;
	m_pos = sf::Vector2f(x, y);

	m_type = static_cast<Type>(type);

	*dataStream >> nInputs;

	for (int i = 0; i < nInputs; ++i)
	{
		auto widget = new Widget(this, QString(), Widget::INPUT, DataType::DATA_BOOLEAN);
		widget->Load(dataStream);

		AddWidget(widget);
	}

	*dataStream >> nOutputs;

	for (int i = 0; i < nOutputs; ++i)
	{
		auto widget = new Widget(this, QString(), Widget::OUTPUT, DataType::DATA_BOOLEAN);
		widget->Load(dataStream);

		AddWidget(widget);
	}
}

void Node::Save(QDataStream *const dataStream)
{
	*dataStream << m_id << m_pos.x << m_pos.y << m_name << m_type << m_execFrom << m_idExecFrom << m_execTo << m_idExecTo;
	*dataStream << m_inputs.size();

	for (auto widget : m_inputs)
	{
		widget->Save(dataStream);
	}

	*dataStream << m_outputs.size();

	for (auto widget : m_outputs)
	{
		widget->Save(dataStream);
	}
}
