/*
*********************************************************************
* File          : NodesWindow.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <QDialog>
#include <ui_NodesWindow.h>

class NodesWindow : public QDialog
{
	Q_OBJECT

private:
	Ui::NodesWindow m_ui;

protected:
	bool event(QEvent *e) override;

public:
	NodesWindow(QWidget *parent);
	~NodesWindow();
};
