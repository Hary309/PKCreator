/*
*********************************************************************
* File          : BackgroundItemWindow.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <ui_BackgroundWindow.h>

#include <ItemWindow.h>

class BackgroundItem;

class BackgroundItemWindow : public ItemWindow
{
	Q_OBJECT

private:
	Ui::BackgroundWindow	m_ui;
	BackgroundItem			*m_pItemParent;

protected:
	void closeEvent(QCloseEvent *event) override;

public:
	BackgroundItemWindow(QWidget *parent);
	~BackgroundItemWindow();

	bool FillData(Item *item) override;

private slots:
	void OkButton_clicked();
	void LoadBgButton_clicked();
};

