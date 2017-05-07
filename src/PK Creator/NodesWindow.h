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

#include <FunctionDefsMgr.h>

class ObjectItem;
class Var;

class NodesWindow : public QDialog
{
	Q_OBJECT

	class FunctionNodeItem
	{
	public:
		QTreeWidgetItem						*treeItem;
		FunctionDefsMgr::FunctionDef		*nodeDef;
	};

	class VarNodeItem
	{
	public:
		QTreeWidgetItem	*treeItem;
		Var				*var;
	};

private:
	Ui::NodesWindow m_ui;

	QVector <QSharedPointer<FunctionNodeItem>>	m_funcitonNodesWidgetItems;
	QVector <QSharedPointer<VarNodeItem>>		m_varNodeWidgetItems;

	int m_type;
	FunctionNodeItem *m_pSelectedFunctionItem;
	VarNodeItem *m_pSelectedVarItem;

protected:
	bool event(QEvent *e) override;

public:
	NodesWindow(QWidget *parent);
	~NodesWindow();

	void AddDefs(const QVector<QSharedPointer<FunctionDefsMgr::FunctionDef>> *nodesDef);

	void show(ObjectItem *objectItem);

	int GetNodeType() const { return m_type; }
	FunctionNodeItem *GetSelectedFunctionItem() const { return m_pSelectedFunctionItem; }
	VarNodeItem *GetSelectedVarItem() const { return m_pSelectedVarItem; }

private slots:
	void NodesWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
};
