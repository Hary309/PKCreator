#pragma once

#include <QStringList>

enum DataType
{
	INTEGER = 0,
	NUMBER,
	STRING,
	BOOLEAN,
	OBJECTID,
	VECTOR2
};

static const QStringList dataNames = { "Integer", "Number", "String", "Boolean", "ObjectID", "Vector2" };

inline DataType GetDataType(const QString &str)
{
	for (int i = 0; i < dataNames.size(); ++i)
	{
		if (str == dataNames[i])
			return DataType(i);
	}

	return DataType(-1);
}