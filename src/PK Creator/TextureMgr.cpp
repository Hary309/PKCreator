/*
*********************************************************************
* File          : TextureMgr.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "TextureMgr.h"

#include <SFML/Graphics/Texture.hpp>

#include <QFile>
#include <QDebug>

#include <ResourceView.h>
#include <SpriteItem.h>

bool TextureMgr::LoadTexture(SpriteItem *sprite)
{
	if (!sprite)
		return false;

	QString path = ResourceView::Get()->GetMainDir() + sprite->GetTexPath();

	return LoadTexture(sprite->GetName(), path, sprite->GetSize(), sprite->GetCenter());
}

bool TextureMgr::LoadTexture(const QString &name, const QString &path, const QSize &size, const QPoint &center)
{
	qInfo() << QString("Loading texture '%1'...").arg(name);
	for (auto tex : m_textures)
	{
		if (tex->name == name)
		{
			qInfo() << "Texture already is loaded!";

			return false;
		}
	}

	auto pTex = QSharedPointer<sf::Texture>(new sf::Texture());
	
	if (!pTex->create(size.width(), size.height()))
	{
		qCritical() << "Cannot create texture!";

		return false;
	}
	
	if (!pTex->loadFromFile(path.toStdString()))
	{
		qCritical() << "Cannot load texture from file";

		return false;
	}

	auto pTexItem = QSharedPointer<TexInfo>(new TexInfo);

	pTexItem->pTex = pTex;
	pTexItem->name = name;
	pTexItem->center = center;

	m_textures.push_back(pTexItem);

	qInfo() << "Texture loaded!";

	return true;
}

TextureMgr::TexInfo *TextureMgr::GetTexture(const QString &name)
{
	for (auto pTex : m_textures)
	{
		if (pTex)
		{
			if (pTex->name == name)
				return pTex.data();
		}
	}

	return nullptr; 
}
