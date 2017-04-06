#include "EventObjectItemWindow.h"

#include <SFML/Graphics.hpp>

#include <QMessageBox>
#include <QStandardItem>

#include <EventObjectItem.h>

EventObjectItemWindow::EventObjectItemWindow(QWidget *parent)
	: QDialog(parent)
{
	m_pItemParent = nullptr;

	resize(800, 600);

	m_pRenderer = QSharedPointer<sf::RenderWindow>(new sf::RenderWindow(HWND(winId())));

	m_timer.setInterval(10);
	m_timer.start();

	connect(&m_timer, &QTimer::timeout, this, &EventObjectItemWindow::Render);
}

EventObjectItemWindow::~EventObjectItemWindow()
{
	
}

bool EventObjectItemWindow::FillData(EventObjectItem *item)
{
	m_pItemParent = item;

	setWindowTitle(QString::asprintf("%s", m_pItemParent->GetItem()->text().toStdString().c_str()));

	return true;
}

void EventObjectItemWindow::Render()
{
	if (m_pRenderer)
	{
		m_pRenderer->clear(sf::Color(0x21212100));
			
		m_pRenderer->display();
	}
}

void EventObjectItemWindow::closeEvent(QCloseEvent *e)
{

	m_pItemParent->Close();
}
