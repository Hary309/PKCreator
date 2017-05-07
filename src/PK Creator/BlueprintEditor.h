/*
*********************************************************************
* File          : BlueprintEditor.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <QTimer>
#include <QWidget>
#include <QSharedPointer>

#include <SFML\System\Vector2.hpp>

namespace sf
{
	class RenderWindow;
	class RectangleShape;
	class Event;
}

class NodeMgr;
class EventObjectItem;

class BlueprintEditor : public QWidget
{
	Q_OBJECT

private:
	EventObjectItem							*m_pData;
	QSharedPointer<sf::RenderWindow>		m_pRenderer;
	QSharedPointer<NodeMgr>					m_pNodeMgr;
	QTimer									m_timer;
	
	sf::Vector2f							m_basicViewPos;
	sf::Vector2f							m_basicViewSize;

	bool									m_viewMoved;
	bool									m_viewMoving;
	sf::Vector2f							m_startViewPos;
	sf::Vector2f							m_cursorStartPos;

	float									m_scale;

	sf::Vector2f							m_nodePos;

protected:
	void mouseMoveEvent(QMouseEvent *e) override;
	void mousePressEvent(QMouseEvent *e) override;
	void mouseReleaseEvent(QMouseEvent *e) override;
	void wheelEvent(QWheelEvent *e) override;
	void keyPressEvent(QKeyEvent *e) override;
	void keyReleaseEvent(QKeyEvent *e) override;

public:
	explicit BlueprintEditor(QWidget *parent);
	~BlueprintEditor();
	
	void Resize(QSize size);

	void FillData(EventObjectItem *item);

	void Render() const;
	void Event(sf::Event *e);

	void ShowNodesWindow(const sf::Vector2f &nodePos);

	sf::Vector2f GetViewOffset() const;
	float GetScale() const { return m_scale; }

	EventObjectItem *GetData() const { return m_pData; }

	sf::RenderWindow *GetRenderer() const { return m_pRenderer.data(); }

	NodeMgr *GetNodeMgr() const { return m_pNodeMgr.data(); }

private slots:
	void NodesWindow_accepted() const;
};

