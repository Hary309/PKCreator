#pragma once

#include <QDialog>
#include <QTimer>

class QStandardItem;
class QCloseEvent;

class EventItem;

namespace sf
{
	class RenderWindow;
	class RectangleShape;
}

class EventItemWindow : public QDialog
{
	Q_OBJECT

private:
	sf::RenderWindow		*m_pRenderer;
	EventItem				*m_pItemParent;

	QTimer					m_timer;

private:
	void closeEvent(QCloseEvent *e) override;

public:
	explicit EventItemWindow(QWidget *parent);
	~EventItemWindow();

	void FillData(EventItem *item);

	void Render();
};

