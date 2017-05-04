/*
*********************************************************************
* File          : NodeDefsMgr.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <Common.h>

#include <QVector>
#include <QSharedPointer>

class NodeDefsMgr
{
public:
	class Arg
	{
	public:
		DataType type;
		QString name;
	};

	class NodeDef
	{
	public:
		int type;
		QString name;
		QString category;
		QVector<Arg> args;
		Arg returnValue;
		QString jsCode;

		NodeDef()
		{
			type = -1;
			name = "";
			category = "";
			returnValue.type = static_cast<DataType>(-1);
			returnValue.name = "";
			jsCode = "";
		}
	};

private:
	QVector<QSharedPointer<NodeDef>> m_nodesDef;

public:
	NodeDefsMgr();
	~NodeDefsMgr();

	void LoadDefs(const QString &path = "nodes");

	auto GetNodesDef() const { return &m_nodesDef; }
};

