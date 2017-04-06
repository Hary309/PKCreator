#pragma once

#include <QTreeView>
#include <QVector>

#include <QSharedPointer>

class QStandardItemModel;
class QStandardItem;

class Item;

class Config;

class ResourceView : public QTreeView
{
	Q_OBJECT

private:
	QStringList							m_defaultModel;

	QString								m_mainDir;

	// to save
	QSharedPointer<QStandardItemModel>  m_pTreeModel;
	
	QVector<QSharedPointer<Item>>		m_items;
	
	QSharedPointer<Config>				m_pProConfig;

	int									m_lastSpriteID;
	int									m_lastObjectID;
	int									m_lastSceneID;
	
	static ResourceView					*s_pInst;

private:
	void mousePressEvent(QMouseEvent * mouseEvent) override;
	void mouseDoubleClickEvent(QMouseEvent *mouseEvent) override;

	static bool ItemsSort(const Item *item1, const Item *item2);

private slots:
	void ActionAdd_triggered();
	void ActionEdit_triggered();
	void ActionRemove_triggered();

public:
	explicit ResourceView(QWidget *parent);
	virtual ~ResourceView();

	void Setup();
	QStandardItem *InsertRow(QStandardItem *parent, const QString &name);
	void RemoveRow(QStandardItem *item) const;

	void InsertItem(Item *item);

	bool Load(QDataStream *const dataStream, const QString &currPath);
	void Save(QDataStream *const dataStream);

	bool IsNameExists(const QString &name);

	Item *GetItem(const QStandardItem *treeItem);
	Item *GetItem(long long id);
	
	const QString &GetMainDir() const { return m_mainDir; }

	// @type - Item::Type
	QVector <Item*>					GetItemsByType(int type);

	Config		*GetConfig()		const { return m_pProConfig.data(); }

	static ResourceView *Get() { return s_pInst; }

private slots:
	void ResourceView_expanded(const QModelIndex &index) const;
	void ResourceView_collapsed(const QModelIndex &index) const;
};

