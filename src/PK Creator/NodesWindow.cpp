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
	AddVarDefs(objectItem);
	AddInlineVarDefs();

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

	for (auto nodesWidgetItem : m_inlineVarNodeWidgetItems)
	{
		if (nodesWidgetItem->treeItem == item)
		{
			m_pSelectedInlineVarItem = nodesWidgetItem.data();
			m_type = Node::INLINE_VARIABLE;

			accept();
			return;
		}

		m_pSelectedInlineVarItem = nullptr;
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

void NodesWindow::AddInlineVarDefs()
{
	m_inlineVarNodeWidgetItems.clear();

	auto nodesWidget = m_ui.nodesWidget;

	// Keys
	{
		for (int i = 0; i < nodesWidget->topLevelItemCount(); ++i)
		{
			if (nodesWidget->topLevelItem(i)->text(0) == "Keyboard")
			{
				delete nodesWidget->topLevelItem(i);
			}
		}

		QTreeWidgetItem *topLevelItem = new QTreeWidgetItem();
		topLevelItem->setText(0, "Keyboard");
		nodesWidget->addTopLevelItem(topLevelItem);

		// Arrows
		AddKeyDef(topLevelItem, 37, "Left arrow");
		AddKeyDef(topLevelItem, 38, "Up arrow");
		AddKeyDef(topLevelItem, 39, "Right arrow");
		AddKeyDef(topLevelItem, 40, "Down arrow");

		// Numbers
		for (int i = 48; i <= 57; ++i)
			AddKeyDef(topLevelItem, i, QString(i));

		// Alpabet
		for (int i = 65; i <= 90; ++i)
			AddKeyDef(topLevelItem, i, QString(i));

		// Numpad
		for (int i = 96; i <= 105; ++i)
			AddKeyDef(topLevelItem, i, "Numpad " + QString::number(i - 96));
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
			auto treeItemChild = new QTreeWidgetItem();
			treeItemChild->setText(0, "Scene " + scene->GetName());
			topLevelItem->addChild(treeItemChild);

			auto nodeItem = QSharedPointer<InlineVarNodeItem>(new InlineVarNodeItem());
			nodeItem->treeItem = treeItemChild;
			nodeItem->value = scene->GetID();
			nodeItem->name = "Scene " + scene->GetName();
			m_inlineVarNodeWidgetItems.push_back(nodeItem);
		}
	}
}

void NodesWindow::AddKeyDef(QTreeWidgetItem *topLevelItem, int key, const QString &name)
{
	auto treeItemChild = new QTreeWidgetItem();
	treeItemChild->setText(0, name + " key");
	topLevelItem->addChild(treeItemChild);

	auto nodeItem = QSharedPointer<InlineVarNodeItem>(new InlineVarNodeItem());
	nodeItem->treeItem = treeItemChild;
	nodeItem->value = QString::number(key);
	nodeItem->name = name + " key";
	m_inlineVarNodeWidgetItems.push_back(nodeItem);
}

bool NodesWindow::event(QEvent *e)
{
	QDialog::event(e);

	if (e->type() == QEvent::WindowDeactivate)
		close();

	return true;
}
