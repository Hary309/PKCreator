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

class SpriteItem;
class ObjectItem;
class SceneItem;
class GlobalVariablesWindow;

class CodeGenerator
{
protected:
	QString m_path;

public:
	CodeGenerator() { }
	virtual ~CodeGenerator() { }

	virtual void GenerateCanvas(const QString &title, const QSize &windowSize) = 0;
	virtual void GenerateGlobalVars(const GlobalVariablesWindow *globalVarsWnd) = 0;
	virtual void GenerateObject(ObjectItem *object) = 0;
	virtual void GenerateSprite(SpriteItem *sprite) = 0;
	virtual void GenerateScene(SceneItem *scene) = 0;

	virtual void Save() = 0;
};

