#include "Widget.h"


Widget::Widget(Node *parent, const QString &name, ConnectionType connType)
	: m_pParent(parent), m_name(name), m_connType(connType)
{
}

Widget::~Widget()
{
}
