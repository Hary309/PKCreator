/*
*********************************************************************
* File          : HTML5Generator.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <CodeGenerator.h>

class HTML5Generator : public CodeGenerator
{
private:
	QSize m_windowSize;

	// html
	QString	m_htmlCode;

	// javascript
	QString m_global;
	QString m_init;
	QString m_render;

public:
	explicit HTML5Generator(const QString &path);
	~HTML5Generator();

	void GenerateCanvas(const QString &title, const QSize &windowSize) override;
	void GenerateObject(ObjectItem *object) override;
	void GenerateSprite(SpriteItem *sprite) override;
	void GenerateScene(SceneItem *item) override;

	void Save() override;
};

