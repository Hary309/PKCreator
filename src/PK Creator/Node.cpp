/*
*********************************************************************
* File          : Node.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "Node.h"

#include <QDateTime>
#include <QDataStream>

#include <Widget.h>


Node::Node(const QString &name, sf::Vector2f pos)
	: m_name(name), m_pos(pos)
{
	m_idExecFrom = m_idExecTo = 0;

	m_id = qint64(this);

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

	*dataStream >> m_id >> x >> y >> m_name >> m_idExecFrom >> m_idExecTo;
	m_pos = sf::Vector2f(x, y);

	*dataStream >> nInputs;

	for (int i = 0; i < nInputs; ++i)
	{
		auto widget = new Widget(this, QString(), Widget::INPUT);
		widget->Load(dataStream);

		AddWidget(widget);
	}

	*dataStream >> nOutputs;

	for (int i = 0; i < nOutputs; ++i)
	{
		auto widget = new Widget(this, QString(), Widget::OUTPUT);
		widget->Load(dataStream);

		AddWidget(widget);
	}
}

void Node::Save(QDataStream *const dataStream)
{
	*dataStream << m_id << m_pos.x << m_pos.y << m_name << m_idExecFrom << m_idExecTo;
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
