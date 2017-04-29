#pragma once

#include <QDialog>
#include "ui_VarWindow.h"

class VarWindow : public QDialog
{
	Q_OBJECT

public:
	VarWindow(QWidget *parent = Q_NULLPTR);
	~VarWindow();

private:
	Ui::VarWindow ui;
};
