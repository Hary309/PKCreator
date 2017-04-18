/*
*********************************************************************
* File          : EventObjectItemWindow.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "EventObjectItemWindow.h"

#include <QStandardItem>

#include <BlueprintEditor.h>

#include <EventObjectItem.h>

#include <QGridLayout>
#include <QResizeEvent>

EventObjectItemWindow::EventObjectItemWindow(QWidget *parent)
	: QMainWindow(parent)
{
	m_pItemParent = nullptr;

	if (objectName().isEmpty())
		setObjectName(QStringLiteral("EventWindow"));

	resize(800, 600);

	auto centralWidget = new QWidget(this);
	centralWidget->setObjectName(QStringLiteral("centralWidget"));
	setCentralWidget(centralWidget);

	auto gridLayout = new QGridLayout(centralWidget);
	gridLayout->setMargin(0);

	m_pBpEditor = new BlueprintEditor(this);
	gridLayout->addWidget(m_pBpEditor);

	QMetaObject::connectSlotsByName(this);

}

EventObjectItemWindow::~EventObjectItemWindow()
{
}

bool EventObjectItemWindow::FillData(EventObjectItem *item)
{
	m_pItemParent = item;

	m_pBpEditor->FillData(item);

	setWindowTitle(QString::asprintf("%s", m_pItemParent->GetItem()->text().toStdString().c_str()));

	return true;
}

void EventObjectItemWindow::closeEvent(QCloseEvent *e)
{
	m_pItemParent->Close();
}

void EventObjectItemWindow::resizeEvent(QResizeEvent *e)
{
	m_pBpEditor->Resize(e->size());
}
