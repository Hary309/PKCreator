/*
*********************************************************************
* File          : Node.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <QVector>
#include <QSharedPointer>
#include <QString>

#include <SFML/System/Vector2.hpp>

class Widget;
class VisualNode;
class HTML5Generator;
class NodeMgr;
class ObjectItem;

class Node
{
public:
	enum Type
	{
		EVENT = 0,
		FUNCTION,
		VARIABLE,
		INLINE_VARIABLE,
		CONDITION
	};

protected:
	qint64								m_id;

	QString								m_name;

	Type								m_type;

	QString								m_defaultValue; // only for INLINE_VARIABLE and CONDITION

	sf::Vector2f						m_pos;

	bool								m_execFrom;
	qint64								m_idExecFrom;
	bool								m_execTo;
	qint64								m_idExecTo;

	QVector<QSharedPointer<Widget>>		m_inputs;
	QVector<QSharedPointer<Widget>>		m_outputs;

	friend VisualNode;
	friend NodeMgr;
	friend HTML5Generator;
	friend ObjectItem;

private:
	void SetType(Type type);

public:
	Node(const QString &name, sf::Vector2f pos, Type type);
	Node(qint64 id, const QString &name, sf::Vector2f pos, Type type);
	~Node();

	Widget *AddWidget(Widget *widget);
	const QString &GetName() const { return m_name; }
	Type GetType() const { return m_type; }

	const QString &GetDefaultValue() const { return m_defaultValue; }

	void Load(QDataStream *const dataStream);
	void Save(QDataStream *const dataStream);

	qint64 GetID() const { return m_id; }
};
