/*
*********************************************************************
* File          : CodeGenerator.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <QString>

class ObjectItem;
class SceneObject;

class CodeGenerator
{
private:
	QString m_finalHTMLCode;

	QString m_globalVars;
	QString m_init;
	QString m_render;

	QString m_path;

public:
	CodeGenerator(const QString &path);
	~CodeGenerator();

	void GenerateHTML(const QString &title, int width, int height);

	void GenerateObject(ObjectItem *obj);
	void GenerateSceneObject(SceneObject *obj);

	void SaveHTML();
	void SaveJS();
};

