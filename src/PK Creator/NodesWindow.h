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

#include <Common.h>
#include <FunctionDefsMgr.h>

class ObjectItem;
class Var;

class NodesWindow : public QDialog
{
	Q_OBJECT

public:
	class NodeItem
	{
	public:
		QTreeWidgetItem *treeItem;
	};

	class FunctionNodeItem : public NodeItem
	{
	public:
		FunctionDefsMgr::FunctionDef *nodeDef;
	};

	class VarNodeItem : public NodeItem
	{
	public:
		Var	*var;
	};

	class InlineVarNodeItem : public NodeItem
	{
	public:
		DataType	type;
		QString		value;
		QString		name;
	};

	class ConditionItem : public NodeItem
	{
	public:
		QString		name;
		QString		conditionType;
		int			nInputs;
		DataType	dataType;
	};

private:
	Ui::NodesWindow m_ui;

	QVector <QSharedPointer<FunctionNodeItem>>	m_funcitonNodesWidgetItems;
	QVector <QSharedPointer<VarNodeItem>>		m_varNodeWidgetItems;
	QVector <QSharedPointer<InlineVarNodeItem>> m_inlineVarNodeWidgetItems;
	QVector <QSharedPointer<ConditionItem>>		m_conditionNodeWidgetItems;

	int m_type;
	NodeItem		*m_pSelectedItem;

private:
	void AddVarDefs(ObjectItem *objectItem);

	void AddGlobalVarDefs();

	void AddConditionDefs();
	void AddConditionDef(QTreeWidgetItem *topLevelItem, const QString &name, const QString &type, int nInputs, DataType dataType);

	void AddInlineVarDefs(ObjectItem *objectItem);
	void AddInlineVarDef(QTreeWidgetItem *topLevelItem, DataType type, const QString &name, const QString &value, const QString &suffixe = QString(" key"));

protected:
	bool event(QEvent *e) override;

public:
	NodesWindow(QWidget *parent);
	~NodesWindow() = default;

	void AddFuncDefs(const QVector<QSharedPointer<FunctionDefsMgr::FunctionDef>> *nodesDef);

	void show(ObjectItem *objectItem);

	int GetNodeType() const { return m_type; }
	NodeItem *GetSelectedItem() const { return m_pSelectedItem; }


private slots:
	void NodesWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
};
