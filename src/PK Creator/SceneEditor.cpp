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
#include <SceneItemWindow.h>
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

	m_pBackground = nullptr;

	m_multipleAdding = false;

	m_timer.setInterval(30);
	m_timer.start();

	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoSystemBackground);

	setMouseTracking(true);
	setFocusPolicy(Qt::FocusPolicy::StrongFocus);

	m_pRenderer = QSharedPointer<sf::RenderWindow>(new sf::RenderWindow(HWND(winId())));
	m_pTexMgr = QSharedPointer<TextureMgr>(new TextureMgr());

	m_hLine = QSharedPointer<sf::RectangleShape>(new sf::RectangleShape(sf::Vector2f(m_windowSize.width(), 1)));
	m_vLine = QSharedPointer<sf::RectangleShape>(new sf::RectangleShape(sf::Vector2f(1, m_windowSize.height())));

	m_hLine->setFillColor(sf::Color(0, 0, 0));
	m_vLine->setFillColor(sf::Color(0, 0, 0));

	connect(&m_timer, &QTimer::timeout, this, &SceneEditor::Render);
}

void SceneEditor::SetSource(SceneItem *sceneItem)
{
	m_pSceneItem = sceneItem;
	m_pObjects =  &sceneItem->m_objects;

	m_pBackground = reinterpret_cast<SceneItemWindow*>(parent())->GetBgImage();

	for (auto obj : *m_pObjects)
	{
		if (!obj)
			continue;

		if (!obj->pObj)
			continue;
		
		if (!obj->pObj->GetSprite())
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

		if (m_pBackground && *m_pBackground)
		{
			auto texSize = m_pBackground->data()->getTexture()->getSize();

			if (texSize.x > 0 && texSize.y > 0)
			{
				int maxX = m_pSceneItem->m_tileHor ? geometry().width() /  texSize.x : 0;
				int maxY = m_pSceneItem->m_tileVer ? geometry().height() / texSize.y : 0;

				for (int i = 0; i <= maxX; ++i)
				{
					for (int j = 0; j <= maxY; ++j)
					{
						m_pBackground->data()->setPosition(i * texSize.x, j * texSize.y);
						m_pRenderer->draw(**m_pBackground);
					}
				}
			}
		}

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

		if (m_multipleAdding && !m_pObjects->isEmpty())
		{
			auto lastObject = m_pObjects->last();

			QPoint startPos = lastObject->pos;
			QPoint endPos = m_cursorPos;

			QPoint startSector = QPoint(startPos.x() / m_snapX, startPos.y() / m_snapY);
			QPoint endSector = QPoint(endPos.x() / m_snapX, endPos.y() / m_snapY);

			QPoint offsetPos = startPos - endPos;
			QPoint offsetSector = startSector - endSector;

			int len = abs(int(sqrt(pow(double(offsetSector.x()), 2) + pow(double(offsetSector.y()), 2))));

			for (int i = 0; i <= len; i += m_freq)
			{
				if (len == 0)
					break;

				if (i == 0)
					continue;

				float alpha = static_cast<float>(i) / len;

				int x = alpha * offsetPos.x();
				int y = alpha * offsetPos.y();

				lastObject->pSpr->setPosition(startPos.x() - x / m_snapX * m_snapX, startPos.y() - y / m_snapY * m_snapY);

				m_pRenderer->draw(*lastObject->pSpr);
			}
			lastObject->pSpr->setPosition(lastObject->pos.x(), lastObject->pos.y());
		}

		if (m_drawGrid)
		{
			if (m_snapX > 3)
			{
				for (int i = 0; i < m_windowSize.width() / m_snapX + 1; ++i)
				{
					m_vLine->setPosition(i * m_snapX, 0);
					m_pRenderer->draw(*m_vLine);
				}
			}

			if (m_snapY > 3)
			{
				for (int i = 0; i < m_windowSize.height() / m_snapY + 1; ++i)
				{
					m_hLine->setPosition(0, i * m_snapY);
					m_pRenderer->draw(*m_hLine);
				}
			}
		}

		m_pRenderer->display();
	}
}

void SceneEditor::mouseMoveEvent(QMouseEvent *e)
{
	m_cursorPos = e->pos();

	if (m_pCurrObj && m_pCurrObj->pSpr)
	{
		QPoint pos;

		pos.setX(e->pos().x() / m_snapX * m_snapX);
		pos.setY(e->pos().y() / m_snapY * m_snapY);

		m_pCurrObj->pSpr->setPosition(pos.x(), pos.y());

		m_pCurrObj->pos = pos;

		if (!m_pCurrObj->pObj)
			return;

		SceneItemWindow *sceneItemWindow = static_cast<SceneItemWindow*>(parent());
		QString text = m_pCurrObj->pObj->GetName() + " pos: (" + QString::number(pos.x()) + ", " + QString::number(pos.y()) + ")";
		
		sceneItemWindow->SetLabelText(text);
	}
}

SceneObject *SceneEditor::AddObject(const QPoint &pos, sf::Texture *texture, const QPoint &center)
{
	auto sharedObj = QSharedPointer<SceneObject>(new SceneObject());

	sharedObj->pObj = m_pSelectedObj;
	sharedObj->pSpr = QSharedPointer<sf::Sprite>(new sf::Sprite(*texture));
	sharedObj->pSpr->setOrigin(center.x(), center.y());

	QPoint objectPos;

	objectPos.setX(pos.x() / m_snapX * m_snapX);
	objectPos.setY(pos.y() / m_snapY * m_snapY);

	sharedObj->pos = objectPos;
	sharedObj->pSpr->setPosition(objectPos.x(), objectPos.y());

	m_pObjects->push_back(sharedObj);

	return sharedObj.data();
}

void SceneEditor::mousePressEvent(QMouseEvent *e)
{
	if (m_multipleAdding)
	{
		auto lastObject = m_pObjects->last();

		QPoint startPos = lastObject->pos;
		QPoint endPos = m_cursorPos;

		QPoint startSector = QPoint(startPos.x() / m_snapX, startPos.y() / m_snapY);
		QPoint endSector = QPoint(endPos.x() / m_snapX, endPos.y() / m_snapY);

		QPoint offsetPos = startPos - endPos;
		QPoint offsetSector = startSector - endSector;

		int len = abs(int(sqrt(pow(double(offsetSector.x()), 2) + pow(double(offsetSector.y()), 2))));

		TextureMgr::TexInfo *texInfo = m_pTexMgr->GetTexture(lastObject->pObj->GetSprite()->GetName());

		for (int i = 0; i <= len; i += m_freq)
		{
			if (len == 0)
				break;

			float alpha = static_cast<float>(i) / len;

			if (i == 0)
				continue;

			int x = startPos.x() - (alpha * offsetPos.x()) / m_snapX * m_snapX;
			int y = startPos.y() - (alpha * offsetPos.y()) / m_snapY * m_snapY;

			AddObject(QPoint(x, y), texInfo->pTex.data(), texInfo->center);
		}
	}

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

	if (!texInfo->pTex)
		return;

	m_pCurrObj = AddObject(e->pos(), texInfo->pTex.data(), texInfo->center);
}

void SceneEditor::mouseReleaseEvent(QMouseEvent *e)
{
	m_pCurrObj = nullptr;
}

void SceneEditor::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_Shift)
	{
		m_multipleAdding = true;
		m_freq = 1;
	}
}

void SceneEditor::keyReleaseEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_Shift)
	{
		m_multipleAdding = false;
	}
}

void SceneEditor::wheelEvent(QWheelEvent *e)
{
	if (m_multipleAdding)
	{
		if (e->delta() > 0)
			m_freq++;
		else if (e->delta() < 0 && m_freq > 1)
			m_freq--;
	}
}

void SceneEditor::resizeEvent(QResizeEvent *e)
{
	Render();
}
