/*
*********************************************************************
* File          : VarEditWindow.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "VarEditWindow.h"

#include <QMessageBox>

#include <Common.h>

#include <Var.h>

VarEditWindow::VarEditWindow(QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);

	for (int i = 0; i < 4; ++i)
	{
		m_ui.typeComboBox->insertItem(i, dataNames[i]);
	}

	m_accepted = false;

	m_ui.typeComboBox->setCurrentIndex(0);

	m_ui.nameEdit->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9]{1,24}")));

	connect(m_ui.okButton, &QPushButton::clicked, this, &VarEditWindow::OkButton_clicked);
	connect(m_ui.typeComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &VarEditWindow::TypeComboBox_currentIndexChanged);
}

VarEditWindow::~VarEditWindow()
{

}

void VarEditWindow::SetVar(Var *var)
{
	m_ui.nameEdit->setText(var->m_name);
	m_ui.typeComboBox->setCurrentIndex(var->m_type);

	switch (var->m_type)
	{
	case DATA_INTEGER:
		 m_ui.valueEdit->setText(QString::number(var->m_data.integer));
		break;
	case DATA_NUMBER:
		m_ui.valueEdit->setText(QString::number(var->m_data.number));
		break;
	case DATA_STRING:
		m_ui.valueEdit->setText(*var->m_data.string);
		break;
	case DATA_BOOLEAN:
		m_ui.valueEdit->setText(QString::number(var->m_data.boolean));
		break;
	default:;
	}
}

Var *VarEditWindow::GetVar()
{
	if (!m_accepted)
		return nullptr;

	auto var = new Var;
	var->m_name = m_ui.nameEdit->text();
	var->m_type = GetDataType(m_ui.typeComboBox->currentText());

	switch (var->m_type)
	{
	case DATA_INTEGER:
		var->m_data.integer = m_ui.valueEdit->text().toInt();
		break;
	case DATA_NUMBER:
		var->m_data.number = m_ui.valueEdit->text().toDouble();
		break;
	case DATA_STRING:
		var->m_data.string = new QString(m_ui.valueEdit->text());
		break;
	case DATA_BOOLEAN:
		var->m_data.boolean = static_cast<bool>(m_ui.valueEdit->text().toInt());
		break;
	default:;
	}

	return var;
}

void VarEditWindow::TypeComboBox_currentIndexChanged(int index)
{
	m_ui.valueEdit->setText("");

	if (index == DATA_INTEGER)
		m_ui.valueEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]{0,9}")));
	else if (index == DATA_NUMBER)
		m_ui.valueEdit->setValidator(new QRegExpValidator(QRegExp("[0-9.]{0,9}")));
	else if (index == DATA_STRING)
	{
		auto validator = new QRegExpValidator;
		validator->setLocale(QLocale::English);

		m_ui.valueEdit->setValidator(validator);
	}
	else if (index == DATA_BOOLEAN)
		m_ui.valueEdit->setValidator(new QRegExpValidator(QRegExp("[0-1]{0,1}")));
}

void VarEditWindow::OkButton_clicked()
{
	if (m_ui.nameEdit->text().isEmpty())
	{
		QMessageBox::information(this, "Info", "Name cannot be empty!");
		return;
	}

	if (m_ui.valueEdit->text().isEmpty())
	{
		QMessageBox::information(this, "Info", "Value cannot be empty!");
		return;
	}

	if (GetDataType(m_ui.typeComboBox->currentText()) == DATA_BOOLEAN)
	{
		if (m_ui.valueEdit->text() != QString("1"))
		{
			if (m_ui.valueEdit->text() != QString("0"))
			{
				QMessageBox::information(this, "Info", "Boolean accepts only 0 (false) and 1 (true)!");
				return;
			}
		}
	}

	m_accepted = true;

	close();
}
