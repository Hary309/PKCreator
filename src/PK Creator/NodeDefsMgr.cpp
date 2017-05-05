/*
*********************************************************************
* File          : NodeDefsMgr.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "NodeDefsMgr.h"

#include <QDir>
#include <QXmlStreamReader>
#include <QTextStream>

NodeDefsMgr::NodeDefsMgr()
{
}

NodeDefsMgr::~NodeDefsMgr()
{
}

void NodeDefsMgr::LoadDefs(const QString &path)
{
	QDir dir(path);

	auto list = dir.entryList(QDir::Files);

	printf("List:\n");

	for (auto item : list)
	{
		printf("%s\n", item.toStdString().c_str());

		QFile file(path + "\\" + item);

		if (!file.open(QIODevice::ReadOnly))
			continue;

		QXmlStreamReader reader(&file);

		if (reader.readNextStartElement()) 
		{
			if (reader.name() == "node")
			{
				auto def = QSharedPointer<NodeDef>(new NodeDef());

				while (reader.readNextStartElement())
				{
					if (reader.name() == "type")
						def->type = reader.readElementText().toInt();
					else if (reader.name() == "name")
						def->name = reader.readElementText();
					else if (reader.name() == "desc")
						def->desc = reader.readElementText();
					else if (reader.name() == "category")
						def->category = reader.readElementText();
					else if (reader.name() == "args")
					{
						while (reader.readNextStartElement())
						{
							if (reader.name() == "arg")
							{
								Arg arg;

								while (reader.readNextStartElement())
								{
									if (reader.name() == "type")
										arg.type = static_cast<DataType>(reader.readElementText().toInt());
									else if (reader.name() == "name")
										arg.name = reader.readElementText();
								}

								def->args.push_back(arg);
							}
						}
					}
					else if (reader.name() == "returnValue")
					{
						Arg ret;

						while (reader.readNextStartElement())
						{
							if (reader.name() == "type")
								ret.type = static_cast<DataType>(reader.readElementText().toInt());
							else if (reader.name() == "name")
								ret.name = reader.readElementText();
						}

						def->returnValue = ret;
					}
					else if (reader.name() == "jsCode")
						def->jsCode = reader.readElementText();
				}

				m_nodesDef.push_back(def);
			}
			else
				printf("Cannot read %s", item.toStdString().c_str());
		}
	}

	printf("\n");
}
