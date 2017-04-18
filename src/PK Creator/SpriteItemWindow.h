/*
*********************************************************************
* File          : SpriteItemWindow.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <ui_SpriteWindow.h>

#include <ItemWindow.h>

class SpriteItem;

class SpriteItemWindow : public ItemWindow
{
	Q_OBJECT

private:
	Ui::SpriteWindow		m_ui;
	SpriteItem				*m_pItemParent;

protected:
	void closeEvent(QCloseEvent *event) override;

public:
	explicit SpriteItemWindow(QWidget *parent);
	virtual ~SpriteItemWindow();

	bool FillData(Item *item) override;

private slots:
	void OkButton_clicked();
	void CenterButton_clicked();
	void LoadSpriteButton_clicked();

	void CenterXEdit_editingFinished();
	void CenterYEdit_editingFinished();
};

