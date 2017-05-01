/*
*********************************************************************
* File          : Common.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <QStringList>

enum DataType
{
	DATA_INTEGER = 0,
	DATA_NUMBER,
	DATA_STRING,
	DATA_BOOLEAN,
	DATA_OBJECTID
};

static const QStringList dataNames = { "Integer", "Number", "String", "Boolean", "ObjectID" };

inline DataType GetDataType(const QString &str)
{
	for (int i = 0; i < dataNames.size(); ++i)
	{
		if (str == dataNames[i])
			return DataType(i);
	}

	return DataType(-1);
}
