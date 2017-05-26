/*
*********************************************************************
* File          : FontItemWindow.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "FontItemWindow.h"

#include <FontItem.h>
#include <QFontDialog>
#include <QColorDialog>
#include <QMessageBox>

#include <ResourceView.h>

FontItemWindow::FontItemWindow(QWidget *parent)
	: ItemWindow(parent)
{
	m_ui.setupUi(this);

	m_ui.nameEdit->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9]{1,24}")));

	connect(m_ui.okButton, &QPushButton::clicked, this,  &FontItemWindow::OkButton_clicked);
	connect(m_ui.fontButton, &QPushButton::clicked, this, &FontItemWindow::FontButton_clicked);
	connect(m_ui.colorButton, &QPushButton::clicked, this, &FontItemWindow::ColorButton_clicked);

	connect(m_ui.leftRadio, &QRadioButton::clicked, this, [this] { m_pItemParent->m_textAlign = FontItem::LEFT; });
	connect(m_ui.centerRadio, &QRadioButton::clicked, this, [this] { m_pItemParent->m_textAlign = FontItem::CENTER; });
	connect(m_ui.rightRadio, &QRadioButton::clicked, this, [this] { m_pItemParent->m_textAlign = FontItem::RIGHT; });
}

bool FontItemWindow::FillData(Item *item)
{
	m_pItemParent = static_cast<FontItem*>(item);

	m_ui.nameEdit->setText(m_pItemParent->GetName());

	SetFont(m_pItemParent->m_font);
	SetColor(QColor(m_pItemParent->m_color));

	if (m_pItemParent->m_textAlign == FontItem::LEFT)
		m_ui.leftRadio->click();
	else if (m_pItemParent->m_textAlign == FontItem::CENTER)
		m_ui.centerRadio->click();
	else if (m_pItemParent->m_textAlign == FontItem::RIGHT)
		m_ui.rightRadio->click();

	return true;
}

void FontItemWindow::SetFont(const QFont &font)
{
	m_pItemParent->m_font = font;
	m_ui.fontButton->setText(m_pItemParent->GetFontString());
}

void FontItemWindow::SetColor(const QColor &color)
{
	m_pItemParent->m_color = color.rgb();
	QString newColor = QString::asprintf("background-color: rgb(%d, %d, %d);", color.red(), color.green(), color.blue());
	m_ui.colorButton->setStyleSheet(newColor);
}

void FontItemWindow::OkButton_clicked()
{
	QString name = m_ui.nameEdit->text();

	if (ResourceView::Get()->IsNameExists(name) && name != m_pItemParent->m_itemName)
	{
		QMessageBox::information(this, "PK Creator", "This name already exists!");
		return;
	}

	m_pItemParent->SetName(name);
	m_pItemParent->Close();
}

void FontItemWindow::FontButton_clicked()
{
	bool ok;
	QFont font = QFontDialog::getFont(&ok, m_pItemParent->m_font, this);

	if (ok)
		SetFont(font);
}

void FontItemWindow::ColorButton_clicked()
{
	QColorDialog colorDialog;

	colorDialog.setCurrentColor(m_pItemParent->m_color);
	colorDialog.setWindowTitle("Choose font color");
	colorDialog.exec();

	auto color = colorDialog.selectedColor();
	SetColor(color);
}
