/*
*********************************************************************
* File          : NodesWindow.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "NodesWindow.h"

#include <QEvent>

NodesWindow::NodesWindow(QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
}

NodesWindow::~NodesWindow()
{
}

bool NodesWindow::event(QEvent *e)
{
	QDialog::event(e);

	if (e->type() == QEvent::WindowDeactivate)
		close();

	return true;
}
