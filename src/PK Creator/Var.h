#pragma once

#include <Common.h>

class QTreeWidgetItem;

class Var
{
public:
	QString m_name;
	DataType m_type;

	QTreeWidgetItem *m_pItem;

	qint64 m_id; // auto generating

	union
	{
		int integer;
		float number;
		QString *string;
		bool boolean;
	} m_data;

	Var() { m_id = qint64(this); m_data.string = nullptr; m_pItem = nullptr; }
	~Var() { if (m_type == DATA_STRING) delete m_data.string; }
};