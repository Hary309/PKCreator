/*
*********************************************************************
* File          : FontItemWindow.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <ui_FontWindow.h>

#include <ItemWindow.h>

class FontItem;

class FontItemWindow : public ItemWindow
{
	Q_OBJECT

private:
	Ui::FontWindow		m_ui;

	FontItem			*m_pItemParent;

private:
	void SetFont(const QFont &font);
	void SetColor(const QColor &color);

public:
	FontItemWindow(QWidget *parent);
	~FontItemWindow() = default;

	bool FillData(Item *item) override;


private slots:
	void OkButton_clicked();
	void FontButton_clicked();
	void ColorButton_clicked();
};

