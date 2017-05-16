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

	class InlineVarNodeItem
	{
	public:
		QTreeWidgetItem		*treeItem;
		QString				value;
		QString				name;
	};

private:
	Ui::NodesWindow m_ui;

	QVector <QSharedPointer<FunctionNodeItem>>	m_funcitonNodesWidgetItems;
	QVector <QSharedPointer<VarNodeItem>>		m_varNodeWidgetItems;
	QVector <QSharedPointer<InlineVarNodeItem>> m_inlineVarNodeWidgetItems;

	int m_type;
	FunctionNodeItem		*m_pSelectedFunctionItem;
	VarNodeItem				*m_pSelectedVarItem;
	InlineVarNodeItem		*m_pSelectedInlineVarItem;

private:
	void AddVarDefs(ObjectItem *objectItem);
	void AddKeyDef(QTreeWidgetItem *topLevelItem, int key, const QString &name);
	void AddInlineVarDefs();

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
	InlineVarNodeItem *GetSelectedInlineVarItem() const { return m_pSelectedInlineVarItem; }

private slots:
	void NodesWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
};
