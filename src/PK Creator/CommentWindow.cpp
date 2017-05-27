/*
*********************************************************************
* File          : CommentWindow.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "CommentWindow.h"

CommentWindow::CommentWindow(QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);

	m_accepted = false;

	connect(m_ui.okButton, &QPushButton::clicked, this, [this] { m_comment = m_ui.commentText->toPlainText(); m_accepted = true; close(); });
	connect(m_ui.cancelButton, &QPushButton::clicked, this, [this] { close(); });
}
