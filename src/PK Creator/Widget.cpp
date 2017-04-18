/*
*********************************************************************
* File          : Widget.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "Widget.h"

#include <QDataStream>

Widget::Widget(Node *parent, const QString &name, ConnectionType connType)
	: m_pParent(parent), m_name(name), m_connType(connType)
{
	m_id = qint64(this);
}

Widget::~Widget()
{
}

void Widget::Load(QDataStream *const dataStream)
{
	int connType = 0;
	*dataStream >> m_id >> m_name >> connType >> m_connected;

	m_connType = static_cast<ConnectionType>(connType);
}

void Widget::Save(QDataStream *const dataStream)
{
	*dataStream << m_id << m_name << m_connType << m_connected;
}
