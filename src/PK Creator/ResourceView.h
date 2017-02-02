#pragma once

#include <QTreeView>
#include <QVector>

class QStandardItemModel;
class QStandardItem;
class Item;

class ResourceView : public QTreeView
{
	Q_OBJECT

private:
	QStandardItemModel	*m_pTreeModel;
	
	QVector<Item*>		m_items;

	int					m_lastTextureID;
	int					m_lastObjectID;

	QStringList			m_defaultModel;

	static ResourceView *s_pInst;

private:
	void mousePressEvent(QMouseEvent * mouseEvent) override;
	void mouseDoubleClickEvent(QMouseEvent *mouseEvent) override;

private slots:
	void ActionAdd_triggered();
	void ActionEdit_triggered();
	void ActionRemove_triggered();

public:
	ResourceView(QWidget *parent);
	virtual ~ResourceView();

	void Setup();
	QStandardItem *InsertRow(QStandardItem *parent, const QString &name);
	void RemoveRow(QStandardItem *item);

	bool IsNameExists(const QString &name);

	Item *GetItem(const QStandardItem *treeItem);


	static ResourceView *Get() { return s_pInst; }
};

