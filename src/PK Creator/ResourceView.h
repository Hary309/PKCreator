#pragma once

#include <QTreeView>
#include <QVector>


class QStandardItemModel;
class QStandardItem;

class Item;

class LuaDebugger;

class Config;

class ResourceView : public QTreeView
{
	Q_OBJECT

private:
	QStringList			m_defaultModel;

	QString				m_mainDir;

	LuaDebugger			*m_pLuaDebugger;

	// to save
	QStandardItemModel	*m_pTreeModel;
	
	QVector<Item*>		m_items;
	
	Config				*m_pProConfig;

	int					m_lastTextureID;
	int					m_lastSpriteID;
	int					m_lastObjectID;
	int					m_lastSceneID;
	
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

	void Load(QDataStream *const dataStream, const QString &currPath);
	void Save(QDataStream *const dataStream);

	bool IsNameExists(const QString &name);

	Item *GetItem(const QStandardItem *treeItem);
	Item *GetItem(const QString &name);

	const QString &GetMainDir() { return m_mainDir; }

	// @type - Item::Type
	QVector <Item*> GetItemsByType(int type);

	LuaDebugger *GetLuaDebugger() { return m_pLuaDebugger; }

	static ResourceView *Get() { return s_pInst; }

private slots:
	void ResourceView_expanded(const QModelIndex &index);
	void ResourceView_collapsed(const QModelIndex &index);
};

