/*
*********************************************************************
* File          : NodesWindow.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "NodesWindow.h"

#include <ResourceView.h>
#include <ObjectItem.h>
#include <Var.h>
#include <Node.h>
#include <GlobalVariablesWindow.h>

#include <QEvent>

NodesWindow::NodesWindow(QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);

	m_type = -1;
	m_pSelectedItem = nullptr;

	AddConditionDefs();

	connect(m_ui.nodesWidget, &QTreeWidget::itemDoubleClicked, this, &NodesWindow::NodesWidget_itemDoubleClicked);
}

void NodesWindow::AddFuncDefs(const QVector<QSharedPointer<FunctionDefsMgr::FunctionDef>> *nodesDef)
{
	for (auto sharedNodeDef : *nodesDef)
	{
		QTreeWidgetItem *topLevelItem = nullptr;

		auto nodesWidget = m_ui.nodesWidget;

		for (int i = 0; i < nodesWidget->topLevelItemCount(); ++i)
		{
			if (nodesWidget->topLevelItem(i)->text(0) == sharedNodeDef->category)
			{
				topLevelItem = nodesWidget->topLevelItem(i);
				break;
			}
		}

		if (!topLevelItem)
		{
			topLevelItem = new QTreeWidgetItem();
			topLevelItem->setText(0, sharedNodeDef->category);
			nodesWidget->addTopLevelItem(topLevelItem);
		}

		auto treeItemChild = new QTreeWidgetItem();
		treeItemChild->setText(0, sharedNodeDef->name);
		treeItemChild->setToolTip(0, sharedNodeDef->desc);
		topLevelItem->addChild(treeItemChild);

		auto nodeItem = QSharedPointer<FunctionNodeItem>(new FunctionNodeItem());
		nodeItem->treeItem = treeItemChild;
		nodeItem->nodeDef = sharedNodeDef.data();

		m_funcitonNodesWidgetItems.push_back(nodeItem);
	}
}

void NodesWindow::show(ObjectItem *objectItem)
{
	AddVarDefs(objectItem);
	AddGlobalVarDefs();
	AddInlineVarDefs(objectItem);

	QDialog::show();
}

void NodesWindow::NodesWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
	for (auto nodesWidgetItem : m_funcitonNodesWidgetItems)
	{
		if (nodesWidgetItem->treeItem == item)
		{
			m_pSelectedItem = nodesWidgetItem.data();
			m_type = Node::FUNCTION;

			accept();
			return;
		}

		m_pSelectedItem = nullptr;
	}

	for (auto nodesWidgetItem : m_varNodeWidgetItems)
	{
		if (nodesWidgetItem->treeItem == item)
		{
			m_pSelectedItem = nodesWidgetItem.data();
			m_type = Node::VARIABLE;

			accept();
			return;
		}
		
		m_pSelectedItem = nullptr;
	}

	for (auto nodesWidgetItem : m_inlineVarNodeWidgetItems)
	{
		if (nodesWidgetItem->treeItem == item)
		{
			m_pSelectedItem = nodesWidgetItem.data();
			m_type = Node::INLINE_VARIABLE;

			accept();
			return;
		}

		m_pSelectedItem = nullptr;
	}

	for (auto nodesWidgetItem : m_conditionNodeWidgetItems)
	{
		if (nodesWidgetItem->treeItem == item)
		{
			m_pSelectedItem = nodesWidgetItem.data();
			m_type = Node::CONDITION;

			accept();
			return;
		}

		m_pSelectedItem = nullptr;
	}
}

void NodesWindow::AddVarDefs(ObjectItem *objectItem)
{
	m_varNodeWidgetItems.clear();

	auto nodesWidget = m_ui.nodesWidget;

	for (int i = 0; i < nodesWidget->topLevelItemCount(); ++i)
	{
		if (nodesWidget->topLevelItem(i)->text(0) == "Variables")
		{
			delete nodesWidget->topLevelItem(i);
		}
	}

	QTreeWidgetItem *topLevelItem = new QTreeWidgetItem();
	topLevelItem->setText(0, "Variables");
	nodesWidget->addTopLevelItem(topLevelItem);

	auto vars = objectItem->GetVars();

	for (auto var : *vars)
	{
		auto treeItemChild = new QTreeWidgetItem();
		treeItemChild->setText(0, var->m_name);
		topLevelItem->addChild(treeItemChild);

		auto nodeItem = QSharedPointer<VarNodeItem>(new VarNodeItem());
		nodeItem->treeItem = treeItemChild;
		nodeItem->var = var.data();
		m_varNodeWidgetItems.push_back(nodeItem);
	}
}

void NodesWindow::AddInlineVarDefs(ObjectItem *objectItem)
{
	m_inlineVarNodeWidgetItems.clear();

	auto nodesWidget = m_ui.nodesWidget;

	// Keys
	{
		QTreeWidgetItem *topLevelItem = nullptr;

		for (int i = 0; i < nodesWidget->topLevelItemCount(); ++i)
		{
			if (nodesWidget->topLevelItem(i)->text(0) == "Keyboard")
			{
				topLevelItem = nodesWidget->topLevelItem(i);
			}
		}

		if (!topLevelItem)
		{
			topLevelItem = new QTreeWidgetItem();
			topLevelItem->setText(0, "Keyboard");
			nodesWidget->addTopLevelItem(topLevelItem);

			// Arrows
			AddInlineVarDef(topLevelItem, DATA_INTEGER, QString::number(37), "Left arrow");
			AddInlineVarDef(topLevelItem, DATA_INTEGER, QString::number(38), "Up arrow");
			AddInlineVarDef(topLevelItem, DATA_INTEGER, QString::number(39), "Right arrow");
			AddInlineVarDef(topLevelItem, DATA_INTEGER, QString::number(40), "Down arrow");

			// Numbers
			for (int i = 48; i <= 57; ++i)
				AddInlineVarDef(topLevelItem, DATA_INTEGER, QString::number(i), QString(i));

			// Alpabet
			for (int i = 65; i <= 90; ++i)
				AddInlineVarDef(topLevelItem, DATA_INTEGER, QString::number(i), QString(i));

			// Numpad
			for (int i = 96; i <= 105; ++i)
				AddInlineVarDef(topLevelItem, DATA_INTEGER, QString::number(i), "Numpad " + QString::number(i - 96));
		}
	}

	// Mouse
	{
		QTreeWidgetItem *topLevelItem = nullptr;

		for (int i = 0; i < nodesWidget->topLevelItemCount(); ++i)
		{
			if (nodesWidget->topLevelItem(i)->text(0) == "Mouse")
			{
				topLevelItem = nodesWidget->topLevelItem(i);
			}
		}

		if (!topLevelItem)
		{
			topLevelItem = new QTreeWidgetItem();
			topLevelItem->setText(0, "Mouse");
			nodesWidget->addTopLevelItem(topLevelItem);

			AddInlineVarDef(topLevelItem, DATA_INTEGER, QString::number(0), "Left", " button");
			AddInlineVarDef(topLevelItem, DATA_INTEGER, QString::number(1), "Middle", " button");
			AddInlineVarDef(topLevelItem, DATA_INTEGER, QString::number(2), "Right", " button");
		}
	}

	// Objects
	{
		for (int i = 0; i < nodesWidget->topLevelItemCount(); ++i)
		{
			if (nodesWidget->topLevelItem(i)->text(0) == "Objects")
			{
				delete nodesWidget->topLevelItem(i);
			}
		}

		QTreeWidgetItem *topLevelItem = new QTreeWidgetItem();
		topLevelItem->setText(0, "Objects");
		nodesWidget->addTopLevelItem(topLevelItem);

		auto objects = ResourceView::Get()->GetItemsByType(Item::OBJECT);

		for (auto object : objects)
		{
			AddInlineVarDef(topLevelItem, DATA_ID, QString::number(object->GetID()), "Object ", object->GetName());
		}
	}

	// Scenes
	{
		for (int i = 0; i < nodesWidget->topLevelItemCount(); ++i)
		{
			if (nodesWidget->topLevelItem(i)->text(0) == "Scenes")
			{
				delete nodesWidget->topLevelItem(i);
			}
		}

		QTreeWidgetItem *topLevelItem = new QTreeWidgetItem();
		topLevelItem->setText(0, "Scenes");
		nodesWidget->addTopLevelItem(topLevelItem);

		auto scenes = ResourceView::Get()->GetItemsByType(Item::SCENE);

		for (auto scene : scenes)
		{
			AddInlineVarDef(topLevelItem, DATA_ID, QString::number(scene->GetID()), "Scene ", scene->GetName());
		}
	}
	
	// Collision directions
	{
		QTreeWidgetItem *topLevelItem = nullptr;

		for (int i = 0; i < nodesWidget->topLevelItemCount(); ++i)
		{
			if (nodesWidget->topLevelItem(i)->text(0) == "Collision")
			{
				topLevelItem = nodesWidget->topLevelItem(i);
			}
		}

		if (!topLevelItem)
		{
			topLevelItem = new QTreeWidgetItem();
			topLevelItem->setText(0, "Collision");
			nodesWidget->addTopLevelItem(topLevelItem);

			AddInlineVarDef(topLevelItem, DATA_INTEGER, QString::number(1), "Left", " Side Collision");
			AddInlineVarDef(topLevelItem, DATA_INTEGER, QString::number(2), "Right", " Side Collision");
			AddInlineVarDef(topLevelItem, DATA_INTEGER, QString::number(3), "Top", " Side Collision");
			AddInlineVarDef(topLevelItem, DATA_INTEGER, QString::number(4), "Bottom", " Side Collision");
		}
	}
}

void NodesWindow::AddInlineVarDef(QTreeWidgetItem *topLevelItem, DataType dataType, const QString &value, const QString &name, const QString &suffixe)
{
	auto treeItemChild = new QTreeWidgetItem();
	treeItemChild->setText(0, name + suffixe);
	topLevelItem->addChild(treeItemChild);

	auto nodeItem = QSharedPointer<InlineVarNodeItem>(new InlineVarNodeItem());
	nodeItem->treeItem = treeItemChild;
	nodeItem->type = dataType;
	nodeItem->value = value;
	nodeItem->name = name + suffixe;
	m_inlineVarNodeWidgetItems.push_back(nodeItem);
}

void NodesWindow::AddConditionDefs()
{
	m_inlineVarNodeWidgetItems.clear();

	auto nodesWidget = m_ui.nodesWidget;

	for (int i = 0; i < nodesWidget->topLevelItemCount(); ++i)
	{
		if (nodesWidget->topLevelItem(i)->text(0) == "Conditions")
			return;
	}

	QTreeWidgetItem *topLevelItem = new QTreeWidgetItem();
	topLevelItem->setText(0, "Conditions");
	nodesWidget->addTopLevelItem(topLevelItem);

	AddConditionDef(topLevelItem, "Is 'a' true", "", 1, DataType::DATA_BOOLEAN);
	AddConditionDef(topLevelItem, "Is 'a' is equal to 'b' (i)", "==", 2, DataType::DATA_INTEGER);
	AddConditionDef(topLevelItem, "Is 'a' is bigger than 'b' (i)", ">", 2, DataType::DATA_INTEGER);
	AddConditionDef(topLevelItem, "Is 'a' is smaller than 'b' (i)", "<", 2, DataType::DATA_INTEGER);

	AddConditionDef(topLevelItem, "Is 'a' is equal to 'b' (n)", "==", 2, DataType::DATA_NUMBER);
	AddConditionDef(topLevelItem, "Is 'a' is bigger than 'b' (n)", ">", 2, DataType::DATA_NUMBER);
	AddConditionDef(topLevelItem, "Is 'a' is smaller than 'b' (n)", "<", 2, DataType::DATA_NUMBER);

	AddConditionDef(topLevelItem, "Is 'a' is equal to 'b' (OID)", "==", 2, DataType::DATA_ID);
}

void NodesWindow::AddConditionDef(QTreeWidgetItem *topLevelItem, const QString &name, const QString &type, int nInputs, DataType dataType)
{
	auto treeItemChild = new QTreeWidgetItem();
	treeItemChild->setText(0, name);
	topLevelItem->addChild(treeItemChild);

	auto nodeItem = QSharedPointer<ConditionItem>(new ConditionItem());
	nodeItem->treeItem = treeItemChild;
	nodeItem->name = name;
	nodeItem->conditionType = type;
	nodeItem->nInputs = nInputs;
	nodeItem->dataType = dataType;
	m_conditionNodeWidgetItems.push_back(nodeItem);
}

void NodesWindow::AddGlobalVarDefs()
{
	auto nodesWidget = m_ui.nodesWidget;

	for (int i = 0; i < nodesWidget->topLevelItemCount(); ++i)
	{
		if (nodesWidget->topLevelItem(i)->text(0) == "Global variables")
		{
			delete nodesWidget->topLevelItem(i);
		}
	}

	QTreeWidgetItem *topLevelItem = new QTreeWidgetItem();
	topLevelItem->setText(0, "Global variables");
	nodesWidget->addTopLevelItem(topLevelItem);

	auto vars = GlobalVariablesWindow::Get()->GetAllVars();

	for (auto var : *vars)
	{
		auto treeItemChild = new QTreeWidgetItem();
		treeItemChild->setText(0, var->m_name);
		topLevelItem->addChild(treeItemChild);

		auto nodeItem = QSharedPointer<VarNodeItem>(new VarNodeItem());
		nodeItem->treeItem = treeItemChild;
		nodeItem->var = var.data();
		m_varNodeWidgetItems.push_back(nodeItem);
	}
}

bool NodesWindow::event(QEvent *e)
{
	QDialog::event(e);

	if (e->type() == QEvent::WindowDeactivate)
		close();

	return true;
}
