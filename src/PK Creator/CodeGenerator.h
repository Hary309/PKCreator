/*
*********************************************************************
* File          : CodeGenerator.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <QString>
#include <QSize>

class ObjectItem;
class SceneObject;

class CodeGenerator
{
protected:
	QString m_path;

public:
	CodeGenerator() { }
	virtual ~CodeGenerator() { }

	virtual void GenerateCanvas(const QString &title, const QSize &windowSize) = 0;

	virtual void GenerateObject(ObjectItem *obj) = 0;
	virtual void GenerateSceneObject(SceneObject *obj) = 0;

	virtual void Save() = 0;
};

