/*
*********************************************************************
* File          : NodesWindow.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "NodesWindow.h"

#include <ResourceView.h>

#include <QEvent>

NodesWindow::NodesWindow(QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);

	m_pSelectedItem = nullptr;

	connect(m_ui.nodesWidget, &QTreeWidget::itemDoubleClicked, this, &NodesWindow::NodesWidget_itemDoubleClicked);
}

NodesWindow::~NodesWindow()
{
}

void NodesWindow::AddDefs(const QVector<QSharedPointer<NodeDefsMgr::NodeDef>> *nodesDef)
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

		topLevelItem->addChild(treeItemChild);

		auto nodeItem = QSharedPointer<NodeItem>(new NodeItem());
		nodeItem->treeItem = treeItemChild;
		nodeItem->nodeDef = sharedNodeDef.data();

		m_nodesWidgetItems.push_back(nodeItem);
	}
}


void NodesWindow::NodesWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
	for (auto sharedNodesWidgetItem : m_nodesWidgetItems)
	{
		if (sharedNodesWidgetItem->treeItem == item)
		{
			m_pSelectedItem = sharedNodesWidgetItem.data();

			accept();
			break;
		}
		else
			m_pSelectedItem = nullptr;
	}
}

bool NodesWindow::event(QEvent *e)
{
	QDialog::event(e);

	if (e->type() == QEvent::WindowDeactivate)
		close();

	return true;
}
