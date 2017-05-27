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
	: m_name(name), m_type(type), m_pos(pos)
{
	m_idExecFrom = m_idExecTo = 0;

	m_id = qint64(this);

	SetType(type);
}

Node::Node(qint64 id, const QString &name, sf::Vector2f pos, Type type)
	: m_id(id), m_name(name), m_type(type), m_pos(pos)
{
	m_idExecFrom = m_idExecTo = 0;

	SetType(type);
}

Node::~Node()
{
}

void Node::SetType(Type type)
{
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
	else if (type == VARIABLE || type == COMMENT)
	{
		m_execFrom = false;
		m_execTo = false;
	}
	else if (type == INLINE_VARIABLE)
	{
		m_execFrom = false;
		m_execTo = false;
	}
	else if (type == CONDITION)
	{
		m_execFrom = true;
		m_execTo = true;
	}
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

	if (m_type == INLINE_VARIABLE || m_type == CONDITION)
		*dataStream >> m_additionalData;

	*dataStream >> nInputs;

	for (int i = 0; i < nInputs; ++i)
	{
		auto widget = new Widget(this, QString(), Widget::DATA, Widget::INPUT, DataType::DATA_BOOLEAN);
		widget->Load(dataStream);

		AddWidget(widget);
	}

	*dataStream >> nOutputs;

	for (int i = 0; i < nOutputs; ++i)
	{
		auto widget = new Widget(this, QString(), Widget::DATA, Widget::OUTPUT, DataType::DATA_BOOLEAN);
		widget->Load(dataStream);

		AddWidget(widget);
	}
}

void Node::Save(QDataStream *const dataStream)
{
	*dataStream << m_id << m_pos.x << m_pos.y << m_name << m_type << m_execFrom << m_idExecFrom << m_execTo << m_idExecTo;

	if (m_type == INLINE_VARIABLE || m_type == CONDITION)
		*dataStream << m_additionalData;

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
