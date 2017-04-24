/*
*********************************************************************
* File          : Config.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include "ui_ConfigWindow.h"

class Config : public QDialog
{
	Q_OBJECT

private:
	Ui::Config m_ui;

public:
	explicit Config(QWidget *parent = Q_NULLPTR);
	~Config();

	void Load(QDataStream *const dataStream) const;
	void Save(QDataStream *const dataStream) const;

	QSize GetWndSize() const;
	QString GetWndTitle();

private slots:
	void OkButton_clicked();
};
