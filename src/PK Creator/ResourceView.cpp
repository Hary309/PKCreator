#include "ResourceView.h"

#include <QString>
#include <QTreeView>
#include <QStandardItemModel>
#include <QMouseEvent>
#include <QMenu>
#include <QCursor>

#include <Texture.h>
#include <Sprite.h>
#include <Object.h>
#include <Scene.h>

#include <LuaDebugger.h>

ResourceView *ResourceView::s_pInst;

ResourceView::ResourceView(QWidget * parent)
	: QTreeView(parent)
{
	m_defaultModel << QString("Textures") << QString("Sprites")
		<< QString("Backgrounds") << QString("Objects") << QString("Scenes");

	Setup();

	m_lastTextureID = 0;
	m_lastSpriteID = 0;
	m_lastObjectID = 0;
	m_lastSceneID = 0;

	m_pLuaDebugger = new LuaDebugger();

	s_pInst = this;
}

ResourceView::~ResourceView()
{
	if (m_pTreeModel)
	{
		delete m_pTreeModel;
		m_pTreeModel = nullptr;
	}

	if (m_pLuaDebugger)
	{
		delete m_pLuaDebugger;
		m_pLuaDebugger = nullptr;
	}

	for (int i = 0; i < m_items.size(); ++i)
	{
		if (m_items[i])
		{
			delete m_items[i];
			m_items[i] = nullptr;
		}
	}

	m_items.clear();
}

void ResourceView::Setup()
{
	m_pTreeModel = new QStandardItemModel();

	setModel(m_pTreeModel);
	setHeaderHidden(true);
	setAnimated(true);

	setMouseTracking(true);

	for (int i = 0; i < m_defaultModel.size(); ++i)
	{
		QStandardItem *item = new QStandardItem(m_defaultModel.at(i));
		item->setEditable(false);

		m_pTreeModel->appendRow(item);
	}
}

void ResourceView::mousePressEvent(QMouseEvent * mouseEvent)
{
	QTreeView::mousePressEvent(mouseEvent);

	if (mouseEvent->button() == Qt::MouseButton::RightButton)
	{
		QModelIndex index = selectionModel()->currentIndex();
		QStandardItem *treeItem = m_pTreeModel->itemFromIndex(index);

		int type = treeItem->parent() ? treeItem->parent()->row() : treeItem->row();

		QMenu contextMenu(tr("Context menu"), this);

		QAction actionAdd(this);
		QAction actionEdit(this);
		QAction actionRemove(this);

		actionAdd.setText(QString::asprintf("Add %s", m_defaultModel.at(type).toStdString().c_str()));
		actionEdit.setText(QString::asprintf("Edit %s", m_defaultModel.at(type).toStdString().c_str()));
		actionRemove.setText(QString::asprintf("Remove %s", treeItem->text().toStdString().c_str()));

		connect(&actionAdd, SIGNAL(triggered()), this, SLOT(ActionAdd_triggered()));
		connect(&actionEdit, SIGNAL(triggered()), this, SLOT(ActionEdit_triggered()));
		connect(&actionRemove, SIGNAL(triggered()), this, SLOT(ActionRemove_triggered()));

		contextMenu.addAction(&actionAdd);

		if (treeItem->parent())
		{
			contextMenu.addAction(&actionEdit);
			contextMenu.addAction(&actionRemove);
		}

		contextMenu.exec(QCursor::pos());
	}
}

void ResourceView::mouseDoubleClickEvent(QMouseEvent *mouseEvent)
{
	if (mouseEvent->button() == Qt::MouseButton::LeftButton)
	{
		QModelIndex index = selectionModel()->currentIndex();

		QStandardItem *treeItem = m_pTreeModel->itemFromIndex(index);

		if (!treeItem->parent())
			return;

		Item *item = GetItem(treeItem);

		if (!item)
			return;

		item->show();
	}
}

bool ResourceView::ItemsSort(const Item *item1, const Item *item2)
{
	if (item1->GetType() < item2->GetType())
		return true;

	if (item1->GetType() > item2->GetType())
		return false;
}

void ResourceView::ActionAdd_triggered()
{
	QModelIndex index = selectionModel()->currentIndex();
	QStandardItem *treeItem = m_pTreeModel->itemFromIndex(index);

	int type = treeItem->parent() ? treeItem->parent()->row() : treeItem->row();
	treeItem = treeItem->parent() ? treeItem->parent() : treeItem;

	QString name;

	switch (type)
	{
		case Item::TEXTURE:
		{
			while (true)
			{
				name.sprintf("texture%d", m_lastTextureID++);

				if (!IsNameExists(name))
					break;
			}
			treeItem = InsertRow(treeItem, name);

			Item *item = new Texture(this, treeItem, name);
			item->show();
			m_items.push_back(item);
		} break;
		case Item::SPRITE:
		{
			while (true)
			{
				name.sprintf("sprite%d", m_lastSpriteID++);

				if (!IsNameExists(name))
					break;
			}
			treeItem = InsertRow(treeItem, name);

			Item *item = new Sprite(this, treeItem, name);
			item->show();
			m_items.push_back(item);
		} break;
		case Item::BACKGROUND:
		{

		} break;
		case Item::OBJECT:
		{
			while (true)
			{
				name.sprintf("object%d", m_lastObjectID++);
			
				if (!IsNameExists(name))
					break;
			}
			treeItem = InsertRow(treeItem, name);

			Item *item = new Object(this, treeItem, name);
			item->show();
			m_items.push_back(item);
		} break;
		case Item::SCENE:
		{
			while (true)
			{
				name.sprintf("scene%d", m_lastSceneID++);

				if (!IsNameExists(name))
					break;
			}
			treeItem = InsertRow(treeItem, name);

			Item *item = new Scene(this, treeItem, name);
			item->show();
			m_items.push_back(item);
		} break;
	}
}

void ResourceView::ActionEdit_triggered()
{
	QModelIndex index = selectionModel()->currentIndex();
	QStandardItem *treeItem = m_pTreeModel->itemFromIndex(index);

	if (!treeItem->parent())
		return;

	Item *item = GetItem(treeItem);

	if (!item)
		return;

	item->show();
}

void ResourceView::ActionRemove_triggered()
{
	QModelIndex index = selectionModel()->currentIndex();
	QStandardItem *treeItem = m_pTreeModel->itemFromIndex(index);

	if (!treeItem->parent())
		return;

	RemoveRow(treeItem);

	for (int i = 0; i < m_items.size(); ++i)
	{
		if (m_items[i]->GetItem() == treeItem)
		{
			delete m_items[i];
			m_items[i] = nullptr;
			m_items.removeAt(i);
		}
	}
}

QStandardItem *ResourceView::InsertRow(QStandardItem *parent, const QString & name)
{
	QModelIndex index = m_pTreeModel->indexFromItem(parent);

	QStandardItem *child = new QStandardItem(name);
	child->setEditable(false);
	parent->appendRow(child);

	expand(index);

	return child;
}

void ResourceView::RemoveRow(QStandardItem *item)
{
	if (item->parent() == 0)
		return;

	m_pTreeModel->removeRow(item->row(), item->parent()->index());
}

void ResourceView::InsertItem(Item *item)
{
	if (!item)
		return;

	m_items.push_back(item);
}

void ResourceView::Load(QDataStream *const dataStream, const QString &currPath)
{
	*dataStream >> m_lastTextureID >> m_lastSpriteID >> m_lastObjectID;

	m_mainDir = currPath;

	printf("%d %d %d\n", m_lastTextureID, m_lastSpriteID, m_lastObjectID);

	int size = 0;

	*dataStream >> size;

	printf("Size: %d\n", size);

	for (int i = 0; i < size; ++i)
	{
		int type = 0;
		QString name;

		*dataStream >> type >> name;

		printf("\n[%d] Type: %d Name: \"%s\"\n", i, type, name.toStdString().c_str());

		QStandardItem *treeItem = m_pTreeModel->item(type);

		treeItem = InsertRow(treeItem, name);

		Item *item = nullptr;

		switch (type)
		{
			case Item::TEXTURE:
			{
				item = new Texture(this, treeItem, name);
			} break;
			case Item::SPRITE:
			{
				item = new Sprite(this, treeItem, name);
			} break;
			case Item::BACKGROUND:
			{

			} break;
			case Item::OBJECT:
			{
				item = new Object(this, treeItem, name);
			} break;
			case Item::SCENE:
			{

			} break;
		}

		if (!item)
			return;

		item->Load(dataStream);
		item->hide();
		m_items.push_back(item);
	}
}

void ResourceView::Save(QDataStream *const dataStream)
{
	*dataStream << m_lastTextureID << m_lastSpriteID << m_lastObjectID; // << m_lastSceneID

	qSort(m_items.begin(), m_items.end(), ItemsSort);

	*dataStream << m_items.size();

	for (int i = 0; i < m_items.size(); ++i)
	{
		if (m_items[i])
		{
			m_items[i]->Save(dataStream);
		}
	}
}

bool ResourceView::IsNameExists(const QString &name)
{
	for (int i = 0; i < m_items.size(); ++i)
	{
		if (m_items[i]->GetName() == name)
			return true;
	}

	return false;
}

Item *ResourceView::GetItem(const QStandardItem *treeItem)
{
	for (int i = 0; i < m_items.size(); ++i)
	{
		if (m_items[i]->GetItem() == treeItem)
		{
			return m_items[i];
		}
	}

	return nullptr;
}

Item *ResourceView::GetItem(const QString &name)
{
	for (int i = 0; i < m_items.size(); ++i)
	{
		if (m_items[i]->GetName() == name)
		{
			return m_items[i];
		}
	}

	return nullptr;
}

QVector<Item*> ResourceView::GetItemsByType(int type)
{
	QVector<Item*> items;

	for (int i = 0; i < m_items.size(); ++i)
	{
		if (m_items[i])
		{
			if (m_items[i]->GetType() == type)
			{
				items.push_back(m_items[i]);
			}
		}
	}

	return items;
}
