#include "EventItemWindow.h"

#include <SFML/Graphics.hpp>

#include <QMessageBox>
#include <QStandardItem>

#include <EventItem.h>

#include <CodeEditor.h>

EventItemWindow::EventItemWindow(QWidget *parent)
	: QDialog(parent)
{
	m_pItemParent = nullptr;

	resize(800, 600);

	m_pRenderer = new sf::RenderWindow(HWND(winId()));

	m_timer.setInterval(10);
	m_timer.start();

	connect(&m_timer, &QTimer::timeout, this, &EventItemWindow::Render);
}

EventItemWindow::~EventItemWindow()
{
	if (m_pRenderer)
	{
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}
}

void EventItemWindow::FillData(EventItem *item)
{
	m_pItemParent = item;

	setWindowTitle(QString::asprintf("%s", m_pItemParent->GetItem()->text().toStdString().c_str()));
}

void EventItemWindow::Render()
{
	if (m_pRenderer)
	{
		m_pRenderer->clear(sf::Color(0x21212100));
			
		m_pRenderer->display();
	}
}

void EventItemWindow::closeEvent(QCloseEvent *e)
{

	m_pItemParent->Close();
}
