#pragma once

#include <QDialog>
#include <QTimer>

#include <QSharedPointer>

class QStandardItem;
class QCloseEvent;

class EventObjectItem;

namespace sf
{
	class RenderWindow;
	class RectangleShape;
}

class EventObjectItemWindow : public QDialog
{
	Q_OBJECT

private:
	QSharedPointer<sf::RenderWindow>		m_pRenderer;
	EventObjectItem							*m_pItemParent;

	QTimer									m_timer;

private:
	void closeEvent(QCloseEvent *e) override;

public:
	explicit EventObjectItemWindow(QWidget *parent);
	~EventObjectItemWindow();

	bool FillData(EventObjectItem *item);

	void Render();
};

