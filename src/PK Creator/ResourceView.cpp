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
	s_pInst = this;
}

ResourceView::~ResourceView()
{
	if (m_pTreeModel)
	{
		delete m_pTreeModel;
		m_pTreeModel = nullptr;
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
