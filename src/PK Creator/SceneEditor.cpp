#include "SceneEditor.h"

#include <SFML/Graphics.hpp>

#include <QDesktopWidget>
#include <QPainter>
#include <QMoveEvent>

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

	m_pWindow = nullptr;
	m_pTexMgr = nullptr;
	m_pCurrObject = nullptr;

	m_snapX = 16;
	m_snapY = 16;

	m_hLine = nullptr;
	m_vLine = nullptr;

	move(QApplication::desktop()->width()  / 2 - width  / 2, 
		 QApplication::desktop()->height() / 2 - height / 2);

	m_timer.setInterval(10);
	m_timer.start();

	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	connect(&m_timer, &QTimer::timeout, this, &SceneEditor::Render);
}

SceneEditor::~SceneEditor()
{
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

	if (m_pCurrObject)
	{
		delete m_pCurrObject;
		m_pCurrObject = nullptr;
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

void SceneEditor::SetCurrObject(const QString &sprName) const
{
	TextureMgr::TexInfo *texInfo = m_pTexMgr->GetTexture(sprName);

	if (!texInfo)
		return;

	m_pCurrObject->setTexture(*texInfo->pTex);
	m_pCurrObject->setOrigin(texInfo->center.x(), texInfo->center.y());
}

void SceneEditor::Pulse()
{
	if (m_pWindow && m_pCurrObject)
	{
	}
}

void SceneEditor::Render()
{
	if (m_pWindow)
	{
		m_pWindow->clear(sf::Color(158, 158, 158));

		if (m_pCurrObject)
			m_pWindow->draw(*m_pCurrObject);

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

		m_pWindow->display();
	}
}

void SceneEditor::mouseMoveEvent(QMouseEvent *e)
{
	if (m_pCurrObject && m_pCurrObject->getTexture())
	{
		QPoint pos;

		pos.setX(e->pos().x() / m_snapX * m_snapX);
		pos.setY(e->pos().y() / m_snapY * m_snapY);

		m_pCurrObject->setPosition(pos.x(), pos.y());
	}
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

	if (!m_pCurrObject)
		m_pCurrObject = new sf::Sprite();

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

	if (m_pCurrObject)
	{
		delete m_pCurrObject;
		m_pCurrObject = nullptr;
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
