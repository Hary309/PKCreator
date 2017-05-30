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
#include <QMenu>

#include <NodesWindow.h>
#include <ResourceView.h>
#include <CommentWindow.h>
#include <EventObjectItem.h>

#include <NodeMgr.h>
#include <Node.h>
#include <Widget.h>

BlueprintEditor::BlueprintEditor(QWidget *parent)
	: QWidget(parent)
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	m_pRenderer = QSharedPointer<sf::RenderWindow>(new sf::RenderWindow(HWND(winId()), settings));
	m_basicViewPos = m_pRenderer->getView().getCenter();
	m_basicViewSize = m_pRenderer->getView().getSize();
	m_scale = 1.f;

	m_timer.setInterval(24); // ~30 fps
	m_timer.start();

	m_pNodeMgr = nullptr;

	m_viewMoving = false;
	m_viewMoved = false;

	m_waitingForNewNode = false;
	setStyleSheet("background-color: #212121;");

	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoSystemBackground);
	setMouseTracking(true);
	setFocusPolicy(Qt::FocusPolicy::StrongFocus);

	connect(&m_timer, &QTimer::timeout, this, &BlueprintEditor::Render);
	connect(ResourceView::Get()->GetNodesWindow(), &QDialog::accepted, this, &BlueprintEditor::NodesWindow_accepted);
}

void BlueprintEditor::Resize(QSize size)
{
	// prevent deformation
	m_pRenderer->setSize(sf::Vector2u(size.width(), size.height()));
	m_pRenderer->setView(sf::View(sf::FloatRect(0.f, 0.f, size.width(), size.height())));

	m_basicViewPos = m_pRenderer->getView().getCenter();
	m_basicViewSize = m_pRenderer->getView().getSize();
	m_scale = 1.f;
}

void BlueprintEditor::FillData(EventObjectItem *item)
{
	m_pData = item;
	m_pNodeMgr = QSharedPointer<NodeMgr>(new NodeMgr(this, &item->m_nodes));

	if (item->m_new)
	{
		item->m_new = false;

		if (item->GetType() >= EventDefsMgr::COLLISION_EVENT)
		{
			Node *node = new Node("Collision event", sf::Vector2f(0.f, 0.f), Node::EVENT);

			node->AddWidget(new Widget(node, "this", Widget::DATA, Widget::OUTPUT, DataType::DATA_ID));
			node->AddWidget(new Widget(node, "Collide with", Widget::DATA, Widget::OUTPUT, DataType::DATA_ID));
			node->AddWidget(new Widget(node, "direction", Widget::DATA, Widget::OUTPUT, DataType::DATA_INTEGER));

			m_pNodeMgr->AddNode(node);
		}
		else
		{
			auto eventDef = EventDefsMgr::Get()->GetEvent(item->m_eventType);

			Node *node = new Node(eventDef->name, sf::Vector2f(0.f, 0.f), Node::EVENT);

			for (auto arg : eventDef->args)
			{
				Widget *widget = new Widget(node, arg.name, Widget::DATA, Widget::OUTPUT, arg.type);
				node->AddWidget(widget);
			}

			m_pNodeMgr->AddNode(node);
		}
	}
}

void BlueprintEditor::Render() const
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
	switch (e->type)
	{
		case sf::Event::MouseButtonPressed:
		{
			if (e->mouseButton.button == sf::Mouse::Right)
			{
				m_startViewPos = m_pRenderer->getView().getCenter();
				m_cursorStartPos = sf::Vector2f(e->mouseButton.x, e->mouseButton.y) * m_scale;
				m_viewMoving = true;
				m_viewMoved = false;
			}
		} break;
		case sf::Event::MouseButtonReleased:
		{
			m_viewMoving = false;

			if (e->mouseButton.button == sf::Mouse::Middle)
			{
				sf::Vector2f viewOffset = GetViewOffset();
				float scale = GetScale();

				auto cursorPos = sf::Vector2f(e->mouseButton.x, e->mouseButton.y) * scale - viewOffset;

				ShowNodesWindow(sf::Vector2f(cursorPos.x, cursorPos.y));
			}
		} break;
		case sf::Event::MouseMoved:
		{
			if (m_viewMoving)
			{
				m_viewMoved = true;

				sf::View view = m_pRenderer->getView();

				view.setCenter((m_startViewPos + m_cursorStartPos) - sf::Vector2f(e->mouseMove.x, e->mouseMove.y) * m_scale);

				m_pRenderer->setView(view);
			}
		} break;
		case sf::Event::MouseWheelMoved:
		{
			sf::View view = m_pRenderer->getView();

			if (e->mouseWheel.delta > 0 && m_scale > 0.75f)
				view.zoom(0.9f);
			else if (e->mouseWheel.delta < 0 && m_scale < 4.f)
				view.zoom(1.1f);

			m_scale = view.getSize().x / m_basicViewSize.x;

			m_pRenderer->setView(view);
		} break;
		case sf::Event::KeyPressed:
		{
			if (e->key.code == Qt::Key::Key_C)
			{
				CommentWindow commentWindow(this);
				commentWindow.exec();

				if (commentWindow.Accepted())
				{
					Node *node = new Node(commentWindow.GetComment(), m_pRenderer->getView().getCenter(), Node::COMMENT);
					m_pNodeMgr->AddNode(node);
				}
			}
		} break;
	default: ;
	}

	if (m_pNodeMgr)
		m_pNodeMgr->Event(e);
}

void BlueprintEditor::ShowNodesWindow(const sf::Vector2f &nodePos)
{
	m_nodePos = nodePos;

	m_waitingForNewNode = true;

	NodesWindow *nodeWnd = ResourceView::Get()->GetNodesWindow();

	nodeWnd->show(m_pData->GetParent());
}

void BlueprintEditor::NodesWindow_accepted()
{
	if (!m_waitingForNewNode)
		return;

	auto nodesWindow = ResourceView::Get()->GetNodesWindow();

	Node::Type type = static_cast<Node::Type>(nodesWindow->GetNodeType());

	NodesWindow::NodeItem *nodeItem = nodesWindow->GetSelectedItem();

	switch (type)
	{
		case Node::FUNCTION:
		{
			NodesWindow::FunctionNodeItem *funcNode = static_cast<NodesWindow::FunctionNodeItem*>(nodeItem);

			if (!funcNode)
				return;

			if (!funcNode->nodeDef)
				return;

			m_pNodeMgr->AddNodeFromFunctionDef(funcNode->nodeDef, m_nodePos);
		} break;
		case Node::VARIABLE:
		{
			NodesWindow::VarNodeItem *varNode = static_cast<NodesWindow::VarNodeItem*>(nodeItem);

			if (!varNode)
				return;

			if (!varNode->var)
				return;

			m_pNodeMgr->AddNodeFromVar(varNode->var, m_nodePos);
		} break;
		case Node::INLINE_VARIABLE:
		{
			NodesWindow::InlineVarNodeItem *inlineVarNode = static_cast<NodesWindow::InlineVarNodeItem*>(nodeItem);

			if (!inlineVarNode)
				return;

			m_pNodeMgr->AddInlineVarNode(inlineVarNode, m_nodePos);
		} break;
		case Node::CONDITION:
		{
			NodesWindow::ConditionItem *conditionItem = static_cast<NodesWindow::ConditionItem*>(nodeItem);

			if (!conditionItem)
				return;

			m_pNodeMgr->AddConditionNode(conditionItem, m_nodePos);

		} break;
	default: ;
	}

	setFocus();

}

sf::Vector2f BlueprintEditor::GetViewOffset() const
{
	return m_basicViewPos - m_pRenderer->getView().getCenter() + (m_pRenderer->getView().getSize() - m_basicViewSize) / 2.f;
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
	else if (e->button() == Qt::RightButton)
		sfEvent.mouseButton.button = sf::Mouse::Right;
	else if (e->button() == Qt::MiddleButton)
		sfEvent.mouseButton.button = sf::Mouse::Middle;

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
	else if (e->button() == Qt::RightButton)
		sfEvent.mouseButton.button = sf::Mouse::Right;
	else if (e->button() == Qt::MiddleButton)
		sfEvent.mouseButton.button = sf::Mouse::Middle;

	Event(&sfEvent);
}

void BlueprintEditor::wheelEvent(QWheelEvent *e)
{
	sf::Event sfEvent;

	sfEvent.type = sf::Event::MouseWheelMoved;
	sfEvent.mouseWheel.delta = e->delta();

	Event(&sfEvent);
}

void BlueprintEditor::keyPressEvent(QKeyEvent *e)
{
	sf::Event sfEvent;

	sfEvent.type = sf::Event::KeyPressed;
	sfEvent.key.code = static_cast<sf::Keyboard::Key>(e->key());
	Event(&sfEvent);
}

void BlueprintEditor::keyReleaseEvent(QKeyEvent *e)
{
	sf::Event sfEvent;

	sfEvent.type = sf::Event::KeyReleased;
	sfEvent.key.code = static_cast<sf::Keyboard::Key>(e->key());
	Event(&sfEvent);
}

void BlueprintEditor::paintEvent(QPaintEvent *e)
{
	Render();
}
