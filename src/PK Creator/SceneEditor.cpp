#include "SceneEditor.h"

#include <SFML/Graphics.hpp>

#include <QDesktopWidget>
#include <QPainter>
#include <QMoveEvent>

#include <Scene.h>
#include <TextureMgr.h>

#include <QOpenGLWidget>

SceneEditor::SceneEditor(QWidget *parent)
	: QDialog(parent)
{
	const int width = 800,
		height = 600;

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

	move(QApplication::desktop()->width()  / 2 - width  / 2, 
		 QApplication::desktop()->height() / 2 - height / 2);

	m_timer.setInterval(10);
	m_timer.start();

	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	connect(&m_timer, &QTimer::timeout, this, &SceneEditor::Render);
}

SceneEditor::~SceneEditor()
{
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

		m_pWindow->display();
	}
}

void SceneEditor::mouseMoveEvent(QMouseEvent *e)
{
	m_pCurrObject->setPosition(e->pos().x(), e->pos().y());
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
	if (!m_pWindow)
		m_pWindow = new sf::RenderWindow(HWND(QDialog::winId()));

	if (!m_pTexMgr)
		m_pTexMgr = new TextureMgr();

	if (!m_pCurrObject)
		m_pCurrObject = new sf::Sprite();
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
}

void SceneEditor::paintEvent(QPaintEvent *e)
{
	Pulse();
	Render();
}
