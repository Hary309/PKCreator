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
#include <Common.h>

class VisualNode;
class Node;
class VisualWidget;
class InputDataWidget;
class OutputDataWidget;
class OutputExecWidget;
class HTML5Generator;
class NodeMgr;

class Widget
{
public:
	enum ConnectionType
	{
		INPUT = 0,
		OUTPUT
	};

	enum Type
	{
		DATA,
		EXEC
	};

private:
	qint64				m_id;

	sf::Color			m_color;

	Type				m_type;

	Node				*m_pParent;

	QString				m_name;

	ConnectionType		m_connType;

	DataType			m_dataType;

	// if m_type == DATA widget's id
	// if m_type == EXEC node's id
	QVector<qint64>		m_connected;

	friend VisualWidget;
	friend InputDataWidget;
	friend OutputDataWidget;
	friend OutputExecWidget;
	friend VisualNode;
	friend HTML5Generator;
	friend NodeMgr;

	void SetDataType(DataType type);

public:
	Widget(Node *parent, const QString &name, Type type, ConnectionType connType, DataType dataType);
	~Widget() = default;

	void Load(QDataStream *const dataStream);
	void Save(QDataStream *const dataStream);

	ConnectionType GetConnType()	const	 { return m_connType; }
	const QString &GetName()		const	 { return m_name; }
	qint64 GetID()					const	 { return m_id; }
	DataType GetDataType()			const	 { return m_dataType; }
	sf::Color GetColor()			const	 { return m_color; }
};
		