#pragma once

#include <QList>
#include <QSize>
#include <QPoint>

namespace sf
{
	class Texture;
}

class Sprite;

class TextureMgr
{
public:
	struct TexInfo
	{
		sf::Texture *pTex;
		QString		name;
		QPoint		center;
	};

private:
	QList <TexInfo*> m_textures;

public:
	TextureMgr();
	~TextureMgr();

	bool LoadTexture(Sprite *sprite);
	bool LoadTexture(const QString &name, const QString &path, const QSize &size, const QPoint &center);
	TexInfo *GetTexture(const QString &name);

	void Reset();
};

