/*
*********************************************************************
* File          : CommentWindow.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <ui_CommentWindow.h>

class CommentWindow : public QDialog
{
	Q_OBJECT

private:
	Ui::CommentWindow	m_ui;
	QString				m_comment;

	bool				m_accepted;

public:
	CommentWindow(QWidget *parent);
	~CommentWindow() = default;

	const QString &GetComment() const { return m_comment; }
	bool Accepted() const { return m_accepted; }
};

