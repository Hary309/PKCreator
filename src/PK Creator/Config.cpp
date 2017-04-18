/*
*********************************************************************
* File          : Config.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "Config.h"

Config::Config(QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);

	if (parent)
		m_ui.titleEdit->setText("Game");

	m_ui.widthEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]{2,4}")));
	m_ui.heightEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]{2,4}")));

	m_ui.widthEdit->setText("800");
	m_ui.heightEdit->setText("600");

	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	connect(m_ui.okButton, &QPushButton::clicked, this, [this] { this->hide(); });
}

Config::~Config()
{

}

void Config::Load(QDataStream *const dataStream) const
{
	QString title, width, height;

	*dataStream >> title >> width >> height;

	printf("\"%s\" %dx%d\n", title.toStdString().c_str(), width.toInt(), height.toInt());

	m_ui.titleEdit->setText(title);
	m_ui.widthEdit->setText(width);
	m_ui.heightEdit->setText(height);
}

void Config::Save(QDataStream *const dataStream) const
{
	*dataStream << m_ui.titleEdit->text() << m_ui.widthEdit->text() << m_ui.heightEdit->text();
}

QSize Config::GetWndSize() const
{
	QSize size;
	size.setWidth(m_ui.widthEdit->text().toInt());
	size.setHeight(m_ui.heightEdit->text().toInt());

	return size;
}

void Config::OkButton_clicked()
{
	hide();
}
