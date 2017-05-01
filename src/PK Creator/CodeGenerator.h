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
class SceneItem;

class CodeGenerator
{
protected:
	QString m_path;

public:
	CodeGenerator() { }
	virtual ~CodeGenerator() { }

	virtual void GenerateCanvas(const QString &title, const QSize &windowSize) = 0;
	virtual void GenerateSprite(SpriteItem *sprite) = 0;
	virtual void GenerateScene(SceneItem *scene) = 0;

	virtual void Save() = 0;
};

