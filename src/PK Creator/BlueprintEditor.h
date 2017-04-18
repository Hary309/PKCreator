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
	QSharedPointer<sf::RenderWindow>		m_pRenderer;
	QSharedPointer<NodeMgr>					m_pNodeMgr;
	QTimer									m_timer;

protected:
	void mouseMoveEvent(QMouseEvent *e) override;
	void mousePressEvent(QMouseEvent *e) override;
	void mouseReleaseEvent(QMouseEvent *e) override;

public:
	explicit BlueprintEditor(QWidget *parent);
	~BlueprintEditor();
	
	void Resize(QSize size);

	void FillData(EventObjectItem *item);

	void Render();
	void Event(sf::Event *e);

	NodeMgr *GetNodeMgr() { return m_pNodeMgr.data(); }
};

