/*
*********************************************************************
* File          : SceneEditor.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "SceneEditor.h"

#include <SFML/Graphics.hpp>

#include <QResizeEvent>

#include <ResourceView.h>
#include <Config.h>
#include <TextureMgr.h>
#include <ObjectItem.h>
#include <SceneItem.h>
#include <SpriteItem.h>

SceneEditor::SceneEditor(QWidget *parent)
	: QWidget(parent)
{
	m_windowSize = ResourceView::Get()->GetConfig()->GetWndSize();

	setFixedSize(m_windowSize);

	m_snapX = 16;
	m_snapY = 16;

	m_drawGrid = true;

	m_pObjects = nullptr;
	m_pCurrObj = nullptr;
	m_pSelectedObj = nullptr;

	m_timer.setInterval(10);
	m_timer.start();

	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoSystemBackground);

	m_pRenderer = QSharedPointer<sf::RenderWindow>(new sf::RenderWindow(HWND(winId())));
	m_pTexMgr = QSharedPointer<TextureMgr>(new TextureMgr());

	m_hLine = QSharedPointer<sf::RectangleShape>(new sf::RectangleShape(sf::Vector2f(m_windowSize.width(), 1)));
	m_vLine = QSharedPointer<sf::RectangleShape>(new sf::RectangleShape(sf::Vector2f(1, m_windowSize.height())));

	m_hLine->setFillColor(sf::Color(0, 0, 0));
	m_vLine->setFillColor(sf::Color(0, 0, 0));

	connect(&m_timer, &QTimer::timeout, this, &SceneEditor::Render);
}

SceneEditor::~SceneEditor()
{
}

void SceneEditor::SetSource(SceneItem *sceneItem)
{
	m_pSceneItem = sceneItem;
	m_pObjects =  &sceneItem->m_objects;

	for (auto obj : *m_pObjects)
	{
		if (!obj)
			continue;

		if (!obj->pObj)
			continue;

		m_pTexMgr->LoadTexture(obj->pObj->GetSprite());

		TextureMgr::TexInfo *texInfo = m_pTexMgr->GetTexture(obj->pObj->GetSprite()->GetName());

		if (!texInfo)
			continue;

		obj->pSpr = QSharedPointer<sf::Sprite>(new sf::Sprite(*texInfo->pTex));
		obj->pSpr->setOrigin(texInfo->center.x(), texInfo->center.y());
		obj->pSpr->setPosition(obj->pos.x(), obj->pos.y());
	}
}

void SceneEditor::Render()
{
	if (m_pRenderer)
	{
		m_pRenderer->clear(sf::Color(m_pSceneItem->m_bgColor));

		if (m_pCurrObj)
		{
			sf::Sprite *spr = m_pCurrObj->pSpr.data();
			if (spr)
			{
				sf::IntRect rect = spr->getTextureRect();

				sf::Vector2f pos = spr->getPosition() - spr->getOrigin();

				sf::Vertex vertex[5];

				// left top
				vertex[0] = sf::Vertex(pos - sf::Vector2f(1.f, 0.f), sf::Color::White);
				
				// right top
				vertex[1] = sf::Vertex(pos + sf::Vector2f(rect.width, 0.f), sf::Color::White);

				// right bottom
				vertex[2] = sf::Vertex(pos + sf::Vector2f(rect.width, rect.height + 1.f), sf::Color::White);

				// left bottom
				vertex[3] = sf::Vertex(pos + sf::Vector2f(-1.f, rect.height + 1.f), sf::Color::White);

				// left top
				vertex[4] = sf::Vertex(pos - sf::Vector2f(1.f, 1.f), sf::Color::White);

				m_pRenderer->draw(vertex, 5, sf::PrimitiveType::LineStrip);
			}
		}

		for (auto sObj : *m_pObjects)
		{
			if (sObj && sObj->pSpr)
			{
				m_pRenderer->draw(*sObj->pSpr);
			}
		}

		if (m_drawGrid)
		{
			if (m_snapX > 1)
			{
				for (int i = 0; i < m_windowSize.height() / m_snapY + 1; ++i)
				{
					m_hLine->setPosition(0, i * m_snapY);
					m_pRenderer->draw(*m_hLine);
				}
			}

			if (m_snapY > 1)
			{
				for (int i = 0; i < m_windowSize.width() / m_snapX + 1; ++i)
				{
					m_vLine->setPosition(i * m_snapX, 0);
					m_pRenderer->draw(*m_vLine);
				}
			}
		}

		m_pRenderer->display();
	}
}

void SceneEditor::mouseMoveEvent(QMouseEvent *e)
{
	if (m_pCurrObj && m_pCurrObj->pSpr)
	{
		QPoint pos;

		pos.setX(e->pos().x() / m_snapX * m_snapX);
		pos.setY(e->pos().y() / m_snapY * m_snapY);

		m_pCurrObj->pSpr->setPosition(pos.x(), pos.y());

		m_pCurrObj->pos = pos;
	}
}

void SceneEditor::mousePressEvent(QMouseEvent *e)
{
	for (int i = 0; i < m_pObjects->size(); ++i)
	{
		auto obj = m_pObjects->at(i);

		if (obj)
		{
			auto spr = obj->pSpr;

			if (spr)
			{
				sf::IntRect rect = spr->getTextureRect();
				sf::Vector2f pos = spr->getPosition() - spr->getOrigin();

				if (e->pos().x() >= int(pos.x) &&
					e->pos().x() <= int(pos.x + rect.width) &&
					e->pos().y() >= int(pos.y) &&
					e->pos().y() <= int(pos.y + rect.height))
				{
					if (e->button() == Qt::MouseButton::LeftButton)
						m_pCurrObj = obj.data();
					else if (e->button() == Qt::MouseButton::RightButton)
					{
						m_pObjects->removeAt(i);
						m_pCurrObj = nullptr;
					}

					return;
				}

			}
		}
	}

	if (!m_pSelectedObj)
		return;

	if (e->button() != Qt::MouseButton::LeftButton)
		return;

	TextureMgr::TexInfo *texInfo = m_pTexMgr->GetTexture(m_pSelectedObj->GetSprite()->GetName());

	// TODO: if texInfo is nullptr load default texture

	if (!texInfo)
		return;

	auto sharedObj = QSharedPointer<SceneObject>(new SceneObject());

	sharedObj->pObj = m_pSelectedObj;
	sharedObj->pSpr = QSharedPointer<sf::Sprite>(new sf::Sprite(*texInfo->pTex));
	sharedObj->pSpr->setOrigin(texInfo->center.x(), texInfo->center.y());

	QPoint pos;

	pos.setX(e->pos().x() / m_snapX * m_snapX);
	pos.setY(e->pos().y() / m_snapY * m_snapY);

	sharedObj->pos = pos;
	sharedObj->pSpr->setPosition(pos.x(), pos.y());

	m_pObjects->push_back(sharedObj);

	m_pCurrObj = sharedObj.data();
}

void SceneEditor::mouseReleaseEvent(QMouseEvent *e)
{
	m_pCurrObj = nullptr;
}

void SceneEditor::resizeEvent(QResizeEvent *e)
{
	Render();
}