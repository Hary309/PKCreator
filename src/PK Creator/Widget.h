#pragma once

#include <QString>
#include <QVector>

class Node;

class Widget
{
public:
	enum ConnectionType
	{
		INPUT = 0,
		OUTPUT
	};

	enum ValueType
	{
		INTEGER = 0,
		NUMBER,
		STRING,
		BOOLEAN
	};

private:
	Node				*m_pParent;

	QString				m_name;

	ConnectionType		m_connType;

	QVector<Widget*>	m_pConnected;

public:
	Widget(Node *parent, const QString &name, ConnectionType connType);
	~Widget();

	ConnectionType GetConnType() const { return m_connType; }
	const QString &GetName() const { return m_name; }
};
