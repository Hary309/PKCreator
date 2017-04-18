/*
*********************************************************************
* File          : TextureMgr.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <QVector>
#include <QSize>
#include <QPoint>

#include <QSharedPointer>

namespace sf
{
	class Texture;
}

class SpriteItem;

class TextureMgr
{
public:
	struct TexInfo
	{
		QSharedPointer<sf::Texture>		pTex;
		QString							name;
		QPoint							center;
	};

private:
	QVector <QSharedPointer<TexInfo>> m_textures;

public:
	TextureMgr();
	~TextureMgr();

	bool LoadTexture(SpriteItem *sprite);
	bool LoadTexture(const QString &name, const QString &path, const QSize &size, const QPoint &center);
	TexInfo *GetTexture(const QString &name);
};
