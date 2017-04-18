/*
*********************************************************************
* File          : BlueprintEditor.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "BlueprintEditor.h"

#include <SFML/Graphics.hpp>

#include <QMouseEvent>

#include <EventObjectItem.h>

#include <NodeMgr.h>
#include <Node.h>
#include <Widget.h>

#include <Windows.h>

BlueprintEditor::BlueprintEditor(QWidget *parent)
	: QWidget(parent)
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	m_pRenderer = QSharedPointer<sf::RenderWindow>(new sf::RenderWindow(HWND(winId()), settings));

	m_timer.setInterval(24); // ~30 fps
	m_timer.start();

	m_pNodeMgr = nullptr;

	setMouseTracking(true);

	connect(&m_timer, &QTimer::timeout, this, &BlueprintEditor::Render);
}

BlueprintEditor::~BlueprintEditor()
{
}

void BlueprintEditor::Resize(QSize size)
{
	m_pRenderer->setSize(sf::Vector2u(size.width(), size.height()));
	m_pRenderer->setView(sf::View(sf::FloatRect(0.f, 0.f, size.width(), size.height())));
}

void BlueprintEditor::FillData(EventObjectItem *item)
{
	m_pNodeMgr = QSharedPointer<NodeMgr>(new NodeMgr(&item->m_nodes));

	auto node = new Node("Object in box", sf::Vector2f(16.f, 16.f));
	node->AddWidget(new Widget(node, "Width", Widget::INPUT));
	node->AddWidget(new Widget(node, "Height", Widget::INPUT));
	node->AddWidget(new Widget(node, "Pos", Widget::INPUT));
	node->AddWidget(new Widget(node, "Return", Widget::OUTPUT));
	m_pNodeMgr->AddNode(node);

	node = new Node("Player", sf::Vector2f(200.f, 16.f));
	node->AddWidget(new Widget(node, "Object", Widget::INPUT));
	node->AddWidget(new Widget(node, "Width", Widget::OUTPUT));
	node->AddWidget(new Widget(node, "Height", Widget::OUTPUT));
	node->AddWidget(new Widget(node, "Pos", Widget::OUTPUT));
	m_pNodeMgr->AddNode(node);
}

void BlueprintEditor::Render()
{
	if (m_pRenderer)
	{
		m_pRenderer->clear(sf::Color(0x21212100));

		if (m_pNodeMgr)
			m_pNodeMgr->Render(m_pRenderer.data());

		m_pRenderer->display();
	}
}

void BlueprintEditor::Event(sf::Event *e)
{
	if (m_pNodeMgr)
		m_pNodeMgr->Event(e);
}

void BlueprintEditor::mouseMoveEvent(QMouseEvent *e)
{
	sf::Event sfEvent;

	sfEvent.type = sf::Event::MouseMoved;
	sfEvent.mouseMove.x = e->x();
	sfEvent.mouseMove.y = e->y();

	Event(&sfEvent);
}

void BlueprintEditor::mousePressEvent(QMouseEvent *e)
{
	sf::Event sfEvent;

	sfEvent.type = sf::Event::MouseButtonPressed;

	sfEvent.mouseButton.x = e->x();
	sfEvent.mouseButton.y = e->y();

	if (e->button() == Qt::LeftButton)
		sfEvent.mouseButton.button = sf::Mouse::Left;

	if (e->button() == Qt::RightButton)
		sfEvent.mouseButton.button = sf::Mouse::Right;

	Event(&sfEvent);
}

void BlueprintEditor::mouseReleaseEvent(QMouseEvent *e)
{
	sf::Event sfEvent;

	sfEvent.type = sf::Event::MouseButtonReleased;
	sfEvent.mouseButton.x = e->x();
	sfEvent.mouseButton.y = e->y();

	if (e->button() == Qt::LeftButton)
		sfEvent.mouseButton.button = sf::Mouse::Left;

	if (e->button() == Qt::RightButton)
		sfEvent.mouseButton.button = sf::Mouse::Right;

	Event(&sfEvent);
}
