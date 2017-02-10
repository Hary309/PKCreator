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
	QStringList			m_defaultModel;

	// to save
	QStandardItemModel	*m_pTreeModel;
	
	QVector<Item*>		m_items;

	int					m_lastTextureID;
	int					m_lastSpriteID;
	int					m_lastObjectID;

	
	static ResourceView *s_pInst;

private:
	void mousePressEvent(QMouseEvent * mouseEvent) override;
	void mouseDoubleClickEvent(QMouseEvent *mouseEvent) override;

	static bool ItemsSort(const Item *item1, const Item *item2);

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

	void InsertItem(Item *item);

	void Load(QDataStream *const dataStream);
	void Save(QDataStream *const dataStream);

	bool IsNameExists(const QString &name);

	Item *GetItem(const QStandardItem *treeItem);
	Item *GetItem(const QString &name);

	// @type - Item::Type
	QVector <Item*> GetItemsByType(int type);

	static ResourceView *Get() { return s_pInst; }
};

