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

	m_pRenderer = new sf::RenderWindow(HWND(winId()));
	m_pTexMgr = new TextureMgr();

	m_hLine = new sf::RectangleShape(sf::Vector2f(1, m_windowSize.height()));
	m_vLine = new sf::RectangleShape(sf::Vector2f(m_windowSize.width(), 1));

	m_hLine->setFillColor(sf::Color(0, 0, 0));
	m_vLine->setFillColor(sf::Color(0, 0, 0));

	connect(&m_timer, &QTimer::timeout, this, &SceneEditor::Render);
}

SceneEditor::~SceneEditor()
{
	if (m_pRenderer)
	{
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	if (m_hLine)
	{
		delete m_hLine;
		m_hLine = nullptr;
	}

	if (m_vLine)
	{
		delete m_vLine;
		m_vLine = nullptr;
	}

	if (m_pTexMgr)
	{
		delete m_pTexMgr;
		m_pTexMgr = nullptr;
	}

	for (SceneObject *obj : *m_pObjects)
	{
		if (obj)
		{
			if (obj->pSpr)
			{
				delete obj->pSpr;
				obj->pSpr = nullptr;
			}
		}
	}
}

void SceneEditor::SetSource(SceneItem *sceneItem)
{
	m_pSceneItem = sceneItem;
	m_pObjects = &sceneItem->m_objects;

	for (auto obj : *m_pObjects)
	{
		m_pTexMgr->LoadTexture(obj->pObj->GetSprite());

		TextureMgr::TexInfo *texInfo = m_pTexMgr->GetTexture(obj->pObj->GetSprite()->GetName());

		if (!texInfo)
			continue;

		obj->pSpr = new sf::Sprite(*texInfo->pTex);
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
			sf::Sprite *spr = m_pCurrObj->pSpr;

			if (spr)
			{
				sf::IntRect rect = spr->getTextureRect();

				int lineWidth = 2;

				sf::RectangleShape verLine(sf::Vector2f(rect.width, lineWidth));
				sf::RectangleShape horLine(sf::Vector2f(lineWidth, rect.height));

				verLine.setFillColor(sf::Color(255, 255, 255));
				horLine.setFillColor(sf::Color(255, 255, 255));

				sf::Vector2f pos = spr->getPosition() - spr->getOrigin();

				// Top
				verLine.setPosition(pos);
				m_pRenderer->draw(verLine);

				// Bottom
				verLine.setPosition(pos + sf::Vector2f(0.f, rect.height));
				m_pRenderer->draw(verLine);

				// Left
				horLine.setPosition(pos);
				m_pRenderer->draw(horLine);

				// Right
				horLine.setPosition(pos + sf::Vector2f(rect.width, 0.f));
				m_pRenderer->draw(horLine);
			}
		}

		for (SceneObject *sObj : *m_pObjects)
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
				for (int i = 0; i < m_windowSize.width() / m_snapX + 1; ++i)
				{
					m_hLine->setPosition(i * m_snapX, 0);
					m_pRenderer->draw(*m_hLine);
				}
			}

			if (m_snapY > 1)
			{
				for (int i = 0; i < m_windowSize.height() / m_snapY + 1; ++i)
				{
					m_vLine->setPosition(0, i * m_snapY);
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
		SceneObject *obj = m_pObjects->at(i);

		if (obj)
		{
			sf::Sprite *spr = obj->pSpr;

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
						m_pCurrObj = obj;
					else if (e->button() == Qt::MouseButton::RightButton)
					{
						delete spr;
						delete obj;

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

	SceneObject *sObj = new SceneObject();

	sObj->pObj = m_pSelectedObj;
	sObj->pSpr = new sf::Sprite(*texInfo->pTex);
	sObj->pSpr->setOrigin(texInfo->center.x(), texInfo->center.y());

	QPoint pos;

	pos.setX(e->pos().x() / m_snapX * m_snapX);
	pos.setY(e->pos().y() / m_snapY * m_snapY);

	sObj->pSpr->setPosition(pos.x(), pos.y());

	m_pObjects->push_back(sObj);

	m_pCurrObj = sObj;
}

void SceneEditor::mouseReleaseEvent(QMouseEvent *e)
{
	m_pCurrObj = nullptr;
}

void SceneEditor::resizeEvent(QResizeEvent *e)
{
	Render();
}