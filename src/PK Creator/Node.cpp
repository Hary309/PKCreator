#include "Node.h"

#include <Widget.h>

Node::Node(const QString &name, sf::Vector2f pos)
	: m_name(name), m_pos(pos)
{
	m_pExecFrom = m_pExecTo = nullptr;
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
