/*
*********************************************************************
* File          : FunctionDefsMgr.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <Common.h>

#include <QVector>
#include <QSharedPointer>

class FunctionDefsMgr
{
public:
	class Arg
	{
	public:
		DataType type;
		QString name;
	};

	class FunctionDef
	{
	public:
		QString name;
		QString desc;
		QString category;
		QVector<Arg> args;
		Arg returnValue;
		QString jsCode;

		FunctionDef()
		{
			name = "";
			category = "";
			returnValue.type = static_cast<DataType>(-1);
			returnValue.name = "";
			jsCode = "";
		}
	};

private:
	QVector<QSharedPointer<FunctionDef>> m_nodesDef;

public:
	FunctionDefsMgr() = default;
	~FunctionDefsMgr() = default;

	void LoadDefs(const QString &path = "modules");

	auto GetFunctionsDef() const { return &m_nodesDef; }
};

