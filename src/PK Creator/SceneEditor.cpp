#include "SceneEditor.h"

#include <SFML/Graphics.hpp>

#include <QDesktopWidget>
#include <QPainter>
#include <QMoveEvent>

#include <Object.h>
#include <Sprite.h>
#include <Scene.h>
#include <TextureMgr.h>
#include <ResourceView.h>
#include <Config.h>

#include <QOpenGLWidget>

SceneEditor::SceneEditor(QWidget *parent)
	: QDialog(parent)
{
	m_windowSize = ResourceView::Get()->GetConfig()->GetWndSize();

	setFixedSize(m_windowSize);

	const int width = m_windowSize.width(),
		height = m_windowSize.height();

	resize(width, height);
	setMinimumSize(QSize(width, height));
	setMaximumSize(QSize(16777215, 16777215));
	setMouseTracking(true);

	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_OpaquePaintEvent);
	setAttribute(Qt::WA_NoSystemBackground);

	m_pWindow		= nullptr;
	m_pTexMgr		= nullptr;
	m_pSelectedObj	= nullptr;
	m_pCurrObj		= nullptr;

	m_snapX = 16;
	m_snapY = 16;

	m_drawGrid = true;

	m_hLine = nullptr;
	m_vLine = nullptr;

	m_bgColor = sf::Color(158, 158, 158).toInteger();

	move(QApplication::desktop()->width()  / 2 - width  / 2, 
		 QApplication::desktop()->height() / 2 - height / 2);

	m_timer.setInterval(10);
	m_timer.start();

	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	connect(&m_timer, &QTimer::timeout, this, &SceneEditor::Render);
}

SceneEditor::~SceneEditor()
{
	for (int i = 0; i < m_objects.size(); ++i)
	{
		if (m_objects[i])
		{
			if (m_objects[i]->pSpr)
			{
				delete m_objects[i]->pSpr;
				m_objects[i]->pSpr = nullptr;
			}

			delete m_objects[i];
			m_objects[i] = nullptr;
		}
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

	if (m_pWindow)
	{
		delete m_pWindow;
		m_pWindow = nullptr;
	}
}

void SceneEditor::SetCurrObject(const Object *obj)
{	
	m_pSelectedObj = obj;
}

void SceneEditor::Pulse()
{
	if (m_pWindow && m_pSelectedObj)
	{
	}
}

void SceneEditor::Render()
{
	if (m_pWindow)
	{
		m_pWindow->clear(sf::Color(m_bgColor));

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
				m_pWindow->draw(verLine);
				
				// Bottom
				verLine.setPosition(pos + sf::Vector2f(0.f, rect.height));
				m_pWindow->draw(verLine);

				// Left
				horLine.setPosition(pos);
				m_pWindow->draw(horLine);
				
				// Right
				horLine.setPosition(pos + sf::Vector2f(rect.width, 0.f));
				m_pWindow->draw(horLine);
			}
		}

		for (SceneObject *sObj : m_objects)
		{
			if (sObj && sObj->pSpr)
			{
				m_pWindow->draw(*sObj->pSpr);
			}
		}

		if (m_drawGrid)
		{
			if (m_snapX > 1)
			{
				for (int i = 0; i < m_windowSize.width() / m_snapX + 1; ++i)
				{
					m_hLine->setPosition(i * m_snapX, 0);
					m_pWindow->draw(*m_hLine);
				}
			}

			if (m_snapY > 1)
			{
				for (int i = 0; i < m_windowSize.height() / m_snapY + 1; ++i)
				{
					m_vLine->setPosition(0, i * m_snapY);
					m_pWindow->draw(*m_vLine);
				}
			}
		}

		m_pWindow->display();
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
	}
}

void SceneEditor::mousePressEvent(QMouseEvent *e)
{
	for (int i = 0; i < m_objects.size(); ++i)
	{
		SceneObject *obj = m_objects[i];

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
						spr = nullptr;
						delete obj;
						obj = nullptr;

						m_objects.removeAt(i);
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

	m_objects.push_back(sObj);

	m_pCurrObj = sObj;
}

void SceneEditor::mouseReleaseEvent(QMouseEvent *e)
{
	m_pCurrObj = nullptr;
}

void SceneEditor::moveEvent(QMoveEvent *e)
{
	QDialog::moveEvent(e);

	if (parent())
	{
		Scene *scene = static_cast<Scene*>(parent());

		QRect rect = scene->geometry();

		const int offsetX = 16;

		scene->move(pos().x() - rect.width() - offsetX, pos().y());
	}
}

void SceneEditor::showEvent(QShowEvent *e)
{
	m_windowSize = ResourceView::Get()->GetConfig()->GetWndSize();

	if (!m_pWindow)
		m_pWindow = new sf::RenderWindow(HWND(QDialog::winId()));

	if (!m_pTexMgr)
		m_pTexMgr = new TextureMgr();

	if (!m_hLine)
		m_hLine = new sf::RectangleShape(sf::Vector2f(1, m_windowSize.height()));
	
	if (!m_vLine)
		m_vLine = new sf::RectangleShape(sf::Vector2f(m_windowSize.width(), 1));

	m_hLine->setFillColor(sf::Color(0, 0, 0));
	m_vLine->setFillColor(sf::Color(0, 0, 0));

	setFixedSize(m_windowSize);
}

void SceneEditor::closeEvent(QCloseEvent *e)
{
	if (parent())
	{
		Scene *scene = static_cast<Scene*>(parent());
		scene->hide();
	}

	if (m_pWindow)
	{
		delete m_pWindow;
		m_pWindow = nullptr;
	}

	if (m_pTexMgr)
	{
		delete m_pTexMgr;
		m_pTexMgr = nullptr;
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
}

void SceneEditor::paintEvent(QPaintEvent *e)
{
	Pulse();
	Render();
}
