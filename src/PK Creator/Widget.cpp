/*
*********************************************************************
* File          : Widget.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "Widget.h"

#include <Common.h>

#include <QDataStream>

Widget::Widget(Node *parent, const QString &name, Type type, ConnectionType connType, DataType dataType)
	: m_pParent(parent), m_name(name), m_type(type), m_connType(connType), m_dataType(dataType)
{
	m_id = qint64(this);

	SetDataType(dataType);
}

void Widget::SetDataType(DataType type)
{
	m_dataType = type;

	// colors source: https://material.io/guidelines/style/color.html
	switch (type)
	{
	case DATA_ID:
		m_color = sf::Color(0xE57373FF);
		break;
	case DATA_BOOLEAN:
		m_color = sf::Color(0x4CAF50FF);
		break;
	case DATA_INTEGER:
		m_color = sf::Color(0x2196F3FF);
		break;
	case DATA_NUMBER:
		m_color = sf::Color(0x9575CDFF);
		break;
	case DATA_STRING:
		m_color = sf::Color(0xFF9800FF);
		break;
	}
}

void Widget::Load(QDataStream *const dataStream)
{
	int type = 0;
	int connType = 0;
	int dataType = 0;
	unsigned color = 0;

	*dataStream >> m_id >> m_name >> type >> connType >> m_connected >> dataType >> color;

	m_type = static_cast<Type>(type);
	m_connType = static_cast<ConnectionType>(connType);
	m_dataType = static_cast<DataType>(dataType);
	m_color = sf::Color(color);
}

void Widget::Save(QDataStream *const dataStream)
{
	*dataStream << m_id << m_name << m_type << m_connType << m_connected << m_dataType << m_color.toInteger();
}
