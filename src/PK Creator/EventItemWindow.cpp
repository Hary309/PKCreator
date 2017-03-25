#include "EventItemWindow.h"

#include <QMessageBox>
#include <QStandardItem>

#include <EventItem.h>

#include <CodeEditor.h>

EventItemWindow::EventItemWindow(QWidget *parent)
	: QDialog(parent)
{
	m_pItemParent = nullptr;
}

EventItemWindow::~EventItemWindow()
{
}

void EventItemWindow::FillData(EventItem *item)
{
	m_pItemParent = item;

	setWindowTitle(QString::asprintf("%s", m_pItemParent->GetItem()->text().toStdString().c_str()));
}

void EventItemWindow::closeEvent(QCloseEvent *e)
{

	m_pItemParent->Close();
}
