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

#include <ResourceView.h>
#include <SpriteItem.h>


TextureMgr::TextureMgr()
{
}

TextureMgr::~TextureMgr()
{
}

bool TextureMgr::LoadTexture(SpriteItem *sprite)
{
	if (!sprite)
		return false;

	QString path = ResourceView::Get()->GetMainDir() + sprite->GetTexPath();

	return LoadTexture(sprite->GetName(), path, sprite->GetSize(), sprite->GetCenter());
}

bool TextureMgr::LoadTexture(const QString &name, const QString &path, const QSize &size, const QPoint &center)
{
	printf("Loading texture \"%s\" \"%s\"... ", name.toStdString().c_str(), path.toStdString().c_str());

	for (auto tex : m_textures)
	{
		if (tex->name == name)
		{
			printf("[EXIST]\n");
			return false;
		}
	}

	auto pTex = QSharedPointer<sf::Texture>(new sf::Texture());
	
	if (!pTex->create(size.width(), size.height()))
	{
		printf("[FAIL]\n");

		return false;
	}
	
	if (!pTex->loadFromFile(path.toStdString()))
	{
		printf("[FAIL]\n");

		return false;
	}

	auto pTexItem = QSharedPointer<TexInfo>(new TexInfo);

	pTexItem->pTex = pTex;
	pTexItem->name = name;
	pTexItem->center = center;

	m_textures.push_back(pTexItem);

	printf("[OK]\n");

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
