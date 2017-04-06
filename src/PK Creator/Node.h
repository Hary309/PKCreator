#pragma once

#include <QPoint>

class Node
{
public:
	enum NodeType
	{
		START,
		FUNCTION,
		VARIABLE
	};

private:
	QPoint m_pos;

	NodeType type;

public:
	Node();
	~Node();

};
