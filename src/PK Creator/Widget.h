/*
*********************************************************************
* File          : Widget.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <QVector>
#include <SFML/Graphics/Color.hpp>

class VisualNode;
class Node;
class VisualWidget;
class InputWidget;
class OutputWidget;

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
	qint64				m_id;

	sf::Color			m_color;

	Node				*m_pParent;

	QString				m_name;

	ConnectionType		m_connType;

	QVector<qint64>		m_connected;

	friend VisualWidget;
	friend InputWidget;
	friend OutputWidget;
	friend VisualNode;

public:
	Widget(Node *parent, const QString &name, ConnectionType connType, sf::Color color = sf::Color::White);
	~Widget();

	void Load(QDataStream *const dataStream);
	void Save(QDataStream *const dataStream);

	ConnectionType GetConnType() const { return m_connType; }
	const QString &GetName() const { return m_name; }
	qint64 GetID() const { return m_id; }
};
		