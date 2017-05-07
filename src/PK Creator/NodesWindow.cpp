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

#include <QEvent>

NodesWindow::NodesWindow(QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);

	m_type = -1;
	m_pSelectedFunctionItem = nullptr;
	m_pSelectedVarItem = nullptr;

	connect(m_ui.nodesWidget, &QTreeWidget::itemDoubleClicked, this, &NodesWindow::NodesWidget_itemDoubleClicked);
}

NodesWindow::~NodesWindow()
{
}

void NodesWindow::AddDefs(const QVector<QSharedPointer<FunctionDefsMgr::FunctionDef>> *nodesDef)
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
	auto nodesWidget = m_ui.nodesWidget;

	m_varNodeWidgetItems.clear();

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

	QDialog::show();
}

void NodesWindow::NodesWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
	for (auto nodesWidgetItem : m_funcitonNodesWidgetItems)
	{
		if (nodesWidgetItem->treeItem == item)
		{
			m_pSelectedFunctionItem = nodesWidgetItem.data();
			m_type = Node::FUNCTION;

			accept();
			return;
		}

		m_pSelectedFunctionItem = nullptr;
	}

	for (auto nodesWidgetItem : m_varNodeWidgetItems)
	{
		if (nodesWidgetItem->treeItem == item)
		{
			m_pSelectedVarItem = nodesWidgetItem.data();
			m_type = Node::VARIABLE;

			accept();
			return;
		}
		
		m_pSelectedVarItem = nullptr;
	}
}

bool NodesWindow::event(QEvent *e)
{
	QDialog::event(e);

	if (e->type() == QEvent::WindowDeactivate)
		close();

	return true;
}
