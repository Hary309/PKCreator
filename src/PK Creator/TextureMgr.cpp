#include "TextureMgr.h"

#include <SFML/Graphics/Texture.hpp>

#include <QFile>

#include <ResourceView.h>
#include <Sprite.h>

TextureMgr::TextureMgr()
{
}

TextureMgr::~TextureMgr()
{
	Reset();
}

bool TextureMgr::LoadTexture(Sprite *sprite)
{
	QString path = ResourceView::Get()->GetMainDir() + sprite->GetTexPath();

	return LoadTexture(sprite->GetName(), path, sprite->GetSize(), sprite->GetCenter());
}

bool TextureMgr::LoadTexture(const QString &name, const QString &path, const QSize &size, const QPoint &center)
{
	printf("Loading texture \"%s\" \"%s\"... ", name.toStdString().c_str(), path.toStdString().c_str());

	for (int i = 0; i < m_textures.size(); ++i)
	{
		if (m_textures[i]->name == name)
		{
			printf("[EXIST]\n");
			return false;
		}
	}

	sf::Texture *pTex = new sf::Texture();
	
	if (!pTex->create(size.width(), size.height()))
	{
		delete pTex;
		pTex = nullptr;

		printf("[FAIL]\n");

		return false;
	}
	
	if (!pTex->loadFromFile(path.toStdString()))
	{
		delete pTex;
		pTex = nullptr;

		printf("[FAIL]\n");

		return false;
	}

	TexInfo *pTexItem = new TexInfo();

	pTexItem->pTex = pTex;
	pTexItem->name = name;
	pTexItem->center = center;

	m_textures.push_back(pTexItem);

	printf("[OK]\n");

	return true;
}

TextureMgr::TexInfo *TextureMgr::GetTexture(const QString &name)
{
	for (TexInfo *pTex : m_textures)
	{
		if (pTex)
		{
			if (pTex->name == name)
				return pTex;
		}
	}

	return nullptr; 
}

void TextureMgr::Reset()
{
	for (int i = 0; i < m_textures.size(); ++i)
	{
		if (m_textures[i])
		{
			if (m_textures[i]->pTex)
			{
				delete m_textures[i]->pTex;
				m_textures[i]->pTex = nullptr;
			}

			delete m_textures[i];
			m_textures[i] = nullptr;
		}
	}

	m_textures.clear();
}
