#pragma once

#include <QDialog>
#include "ui_ConfigWindow.h"

class Config : public QDialog
{
	Q_OBJECT

private:
	Ui::Config ui;

public:
	Config(QWidget *parent = Q_NULLPTR);
	~Config();

	void Load(QDataStream *const dataStream);
	void Save(QDataStream *const dataStream);

private slots:
	void OkButton_clicked();
};
