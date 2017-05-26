/*
*********************************************************************
* File          : FunctionDefsMgr.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "FunctionDefsMgr.h"

#include <QDir>
#include <QXmlStreamReader>
#include <QTextStream>

void FunctionDefsMgr::LoadDefs(const QString &path)
{
	QDir dir(path);

	
	auto dirs = dir.entryList(QDir::Dirs | QDir::NoDotDot);

	printf("Loading modules...");

	for (auto currentDirPath : dirs)
	{
		printf("%s\n", currentDirPath.toStdString().c_str());

		QDir currentDir(path + "\\" + currentDirPath);
		auto files = currentDir.entryList(QDir::Files);

		for (auto currentFileName : files)
		{
			printf("\t%s\n", currentFileName.toStdString().c_str());

			QFile file(path + "\\" + currentDirPath + "\\" + currentFileName);

			if (!file.open(QIODevice::ReadOnly))
				continue;

			QXmlStreamReader reader(&file);

			if (reader.readNextStartElement())
			{
				if (reader.name() == "node")
				{
					auto def = QSharedPointer<FunctionDef>(new FunctionDef());
					def->category = currentDirPath;

					while (reader.readNextStartElement())
					{
						if (reader.name() == "name")
							def->name = reader.readElementText();
						else if (reader.name() == "desc")
							def->desc = reader.readElementText();
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
					printf("\tCannot read %s", currentFileName.toStdString().c_str());
			}

		}
	}

	printf("\n");
}
