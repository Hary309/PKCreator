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

class Node
{
public:
	enum Type
	{
		EVENT = 0,
		FUNCTION
	};

protected:
	qint64								m_id;

	Type								m_type;

	sf::Vector2f						m_pos;

	QString								m_name;

	bool								m_execFrom;
	qint64								m_idExecFrom;
	bool								m_execTo;
	qint64								m_idExecTo;

	QVector<QSharedPointer<Widget>>		m_inputs;
	QVector<QSharedPointer<Widget>>		m_outputs;

	friend VisualNode;

public:
	Node(const QString &name, sf::Vector2f pos, Type type);
	~Node();

	Widget *AddWidget(Widget *widget);
	const QString &GetName() const { return m_name; }

	void Load(QDataStream *const dataStream);
	void Save(QDataStream *const dataStream);

	qint64 GetID() const { return m_id; }
};
