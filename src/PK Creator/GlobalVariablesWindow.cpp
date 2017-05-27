/*
*********************************************************************
* File          : GlobalVariablesWindow.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "GlobalVariablesWindow.h"

#include <QTreeWidgetItem>
#include <QMessageBox>

#include <Var.h>

#include <VarEditWindow.h>

GlobalVariablesWindow *GlobalVariablesWindow::s_pInst;

GlobalVariablesWindow::GlobalVariablesWindow(QWidget *parent)
	: QDialog(parent)
{
	s_pInst = this;

	m_ui.setupUi(this);

	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	connect(m_ui.addButton, &QPushButton::clicked, this, &GlobalVariablesWindow::AddVarButton_clicked);
	connect(m_ui.editButton, &QPushButton::clicked, this, &GlobalVariablesWindow::EditVarButton_clicked);
	connect(m_ui.removeButton, &QPushButton::clicked, this, &GlobalVariablesWindow::RemoveVarButton_clicked);
	connect(m_ui.okButton, &QPushButton::clicked, this, [this] { this->close(); });
	connect(m_ui.varsWidget, &QTreeWidget::doubleClicked, this, [this] { EditVarButton_clicked(); });
}

void GlobalVariablesWindow::AddVar(Var *var)
{
	if (!var)
		return;

	for (auto var_ : m_vars)
	{
		if (var_->m_name == var->m_name)
			return;
	}

	auto sharedVar = QSharedPointer<Var>(var);

	m_vars.push_back(sharedVar);

	AddVarToVisualList(var);
}

void GlobalVariablesWindow::AddVarToVisualList(Var *var) const
{
	auto treeItem = new QTreeWidgetItem();
	treeItem->setText(0, var->m_name);
	treeItem->setText(1, dataNames[var->m_type]);

	var->m_pItem = treeItem;

	switch (var->m_type)
	{
	case DATA_INTEGER:
		treeItem->setText(2, QString::number(var->m_data.integer));
		break;
	case DATA_NUMBER:
		treeItem->setText(2, QString::number(var->m_data.number, 'f', 10));
		break;
	case DATA_STRING:
		treeItem->setText(2, *var->m_data.string);
		break;
	case DATA_BOOLEAN:
		treeItem->setText(2, var->m_data.boolean ? "true" : "false");
		break;
	default:;
	}

	m_ui.varsWidget->insertTopLevelItem(m_ui.varsWidget->topLevelItemCount(), treeItem);
}

Var *GlobalVariablesWindow::GetVar(qint64 id)
{
	for (auto var : m_vars)
	{
		if (var)
		{
			if (var->m_id == id)
				return var.data();
		}
	}

	return nullptr;
}

void GlobalVariablesWindow::Load(QDataStream *const dataStream)
{
	int size;

	*dataStream >> size;

	for (int i = 0; i < size; ++i)
	{
		QString name;
		qint64 id;
		int type;

		*dataStream >> name >> id >> type;

		auto var = new Var();
		var->m_name = name;
		var->m_id = id;
		var->m_type = static_cast<DataType>(type);

		switch (var->m_type)
		{
		case DATA_INTEGER:
			*dataStream >> var->m_data.integer;
			break;
		case DATA_NUMBER:
			*dataStream >> var->m_data.number;
			break;
		case DATA_STRING:
		{
			QString str;

			*dataStream >> str;

			var->m_data.string = new QString(str);
		} break;
		case DATA_BOOLEAN:
			*dataStream >> var->m_data.boolean;
			break;
		default:;
		}

		AddVar(var);
	}
}

void GlobalVariablesWindow::Save(QDataStream *const dataStream)
{
	*dataStream << m_vars.size();

	for (auto var : m_vars)
	{
		*dataStream << var->m_name << var->m_id << var->m_type;

		switch (var->m_type)
		{
		case DATA_INTEGER:
			*dataStream << var->m_data.integer;
			break;
		case DATA_NUMBER:
			*dataStream << var->m_data.number;
			break;
		case DATA_STRING:
			*dataStream << *var->m_data.string;
			break;
		case DATA_BOOLEAN:
			*dataStream << var->m_data.boolean;
			break;
		default:;
		}
	}
}

void GlobalVariablesWindow::AddVarButton_clicked()
{
	VarEditWindow varEdit(this);
	varEdit.exec();

	auto var = varEdit.GetVar();

	if (!var)
		return;

	for (auto var_ : m_vars)
	{
		if (var_->m_name == var->m_name)
		{
			QMessageBox::information(this, "Info", "This variable arleady exists!");
			return;
		}
	}

	AddVar(var);
}

void GlobalVariablesWindow::EditVarButton_clicked()
{
	auto currItem = m_ui.varsWidget->currentItem();

	if (!currItem)
		return;

	for (auto var : m_vars)
	{
		if (var->m_pItem == currItem)
		{
			VarEditWindow varEdit(this);
			varEdit.SetVar(var.data());
			varEdit.exec();

			auto newVar = varEdit.GetVar();

			if (!newVar)
				return;

			if (var->m_name != newVar->m_name)
			{
				for (auto newVar_ : m_vars)
				{
					if (newVar_->m_name == newVar->m_name)
					{
						QMessageBox::information(this, "Info", "This variable arleady exists!");
						return;
					}
				}
			}

			var->m_name = newVar->m_name;
			var->m_type = newVar->m_type;
			var->m_data = newVar->m_data;

			if (var->m_pItem)
			{
				auto treeItem = var->m_pItem;

				treeItem->setText(0, var->m_name);
				treeItem->setText(1, dataNames[var->m_type]);

				var->m_pItem = treeItem;

				switch (var->m_type)
				{
				case DATA_INTEGER:
					treeItem->setText(2, QString::number(var->m_data.integer));
					break;
				case DATA_NUMBER:
					treeItem->setText(2, QString::number(var->m_data.number));
					break;
				case DATA_STRING:
					treeItem->setText(2, *var->m_data.string);
					break;
				case DATA_BOOLEAN:
					treeItem->setText(2, var->m_data.boolean ? "true" : "false");
					break;
				default:;
				}
			}
		}
	}

}

void GlobalVariablesWindow::RemoveVarButton_clicked()
{
	auto currItem = m_ui.varsWidget->currentItem();

	if (!currItem)
		return;

	for (auto var : m_vars)
	{
		if (var->m_pItem == currItem)
		{
			delete var->m_pItem;
			m_vars.removeOne(var);
		}
	}
}
