#pragma once

#include <QDialog>
#include <QTimer>

#include <QSharedPointer>

class QStandardItem;
class QCloseEvent;

class EventObjectItem;

class NodeMgr;

namespace sf
{
	class RenderWindow;
	class RectangleShape;
	class Event;
}

class EventObjectItemWindow : public QDialog
{
	Q_OBJECT

private:
	QSharedPointer<sf::RenderWindow>		m_pRenderer;
	EventObjectItem							*m_pItemParent;

	QSharedPointer<NodeMgr>					m_pNodeMgr;

	QTimer									m_timer;

protected:
	void closeEvent(QCloseEvent *e) override;
	void mouseMoveEvent(QMouseEvent *e) override;
	void mousePressEvent(QMouseEvent *e) override;
	void mouseReleaseEvent(QMouseEvent *e) override;

public:
	explicit EventObjectItemWindow(QWidget *parent);
	~EventObjectItemWindow();

	bool FillData(EventObjectItem *item);

	void Render();
	void Event(sf::Event *e);
};

