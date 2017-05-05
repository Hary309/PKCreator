/*
*********************************************************************
* File          : NodesWindow.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <QDialog>
#include <ui_NodesWindow.h>

#include <NodeDefsMgr.h>

class NodesWindow : public QDialog
{
	Q_OBJECT

	class NodeItem
	{
	public:
		QTreeWidgetItem				*treeItem;
		NodeDefsMgr::NodeDef		*nodeDef;
	};

private:
	Ui::NodesWindow m_ui;

	QVector <QSharedPointer<NodeItem>> m_nodesWidgetItems;

	NodeItem *m_pSelectedItem;

protected:
	bool event(QEvent *e) override;

public:
	NodesWindow(QWidget *parent);
	~NodesWindow();

	void AddDefs(const QVector<QSharedPointer<NodeDefsMgr::NodeDef>> *nodesDef);

	NodeItem *GetSelectedItem() const { return m_pSelectedItem; }

private slots:
	void NodesWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
};
