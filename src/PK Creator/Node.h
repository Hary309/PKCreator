#pragma once

#include <QVector>
#include <QSharedPointer>
#include <QString>

#include <SFML/System/Vector2.hpp>

class Widget;
class VisualNode;

class Node
{
protected:
	sf::Vector2f						m_pos;

	QString								m_name;

	Node								*m_pExecFrom;
	Node								*m_pExecTo;

	QVector<QSharedPointer<Widget>>		m_inputs;
	QVector<QSharedPointer<Widget>>		m_outputs;

	friend VisualNode;

public:
	Node(const QString &name, sf::Vector2f pos);
	~Node();

	Widget *AddWidget(Widget *widget);
	const QString &GetName() const { return m_name; }
};
