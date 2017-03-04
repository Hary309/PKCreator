#pragma once

#include "ui_ConfigWindow.h"

class Config : public QDialog
{
	Q_OBJECT

private:
	Ui::Config ui;

public:
	explicit Config(QWidget *parent = Q_NULLPTR);
	~Config();

	void Load(QDataStream *const dataStream) const;
	void Save(QDataStream *const dataStream) const;

	QSize GetWndSize() const;

private slots:
	void OkButton_clicked();
};
