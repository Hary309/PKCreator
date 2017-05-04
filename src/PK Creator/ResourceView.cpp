/*
*********************************************************************
* File          : ResourceView.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "ResourceView.h"

#include <QString>
#include <QTreeView>
#include <QMouseEvent>
#include <QMenu>
#include <QCursor>

#include <NodesWindow.h>

#include <SpriteItem.h>
#include <ObjectItem.h>
#include <SceneItem.h>
#include <Config.h>

#include <CodeGenerator.h>

#include <QMessageBox>

#include <Windows.h>

ResourceView *ResourceView::s_pInst;

ResourceView::ResourceView(QWidget *parent)
	: QTreeView(parent)
{
	m_defaultModel << QString("Sprites") << QString("Backgrounds") 
				   << QString("Objects") << QString("Scenes");

	Setup();

	m_lastSpriteID = 0;
	m_lastObjectID = 0;
	m_lastSceneID = 0;

	m_pProConfig = QSharedPointer<Config>(new Config(this));

	s_pInst = this;

	m_pNodesWindow = QSharedPointer<NodesWindow>(new NodesWindow(this));

	connect(this, &QTreeView::expanded, this, &ResourceView::ResourceView_expanded);
	connect(this, &QTreeView::collapsed, this, &ResourceView::ResourceView_collapsed);
}

ResourceView::~ResourceView()
{
}

void ResourceView::Setup()
{
	m_pTreeModel = QSharedPointer<QStandardItemModel>(new QStandardItemModel());

	setModel(m_pTreeModel.data());
	setHeaderHidden(true);
	setAnimated(true);

	setMouseTracking(true);
	
	for (auto name : m_defaultModel)
	{
		QStandardItem *item = new QStandardItem(name);

		QIcon icon;
		icon.addFile(":/ResourceView/res/folderCloseIcon.png");

		item->setIcon(icon);
		item->setEditable(false);

		m_pTreeModel->appendRow(item);
	}

	QStandardItem *item = new QStandardItem("Config");
	QIcon icon;
	icon.addFile(":/ResourceView/res/configIcon.png");
	item->setIcon(icon);
	item->setEditable(false);

	m_pTreeModel->appendRow(item);
}

void ResourceView::mousePressEvent(QMouseEvent *mouseEvent)
{
	QTreeView::mousePressEvent(mouseEvent);

	if (mouseEvent->button() == Qt::MouseButton::RightButton)
	{
		QModelIndex index = selectionModel()->currentIndex();
		QStandardItem *treeItem = m_pTreeModel->itemFromIndex(index);

		if (treeItem->text() == QString("Config"))
			return;

		int type = treeItem->parent() ? treeItem->parent()->row() : treeItem->row();

		QMenu contextMenu(tr("Context menu"), this);

		QAction actionAdd(this);
		QAction actionEdit(this);
		QAction actionRemove(this);

		actionAdd.setText(QString::asprintf("Add %s", m_defaultModel.at(type).toStdString().c_str()));
		actionEdit.setText(QString::asprintf("Edit %s", treeItem->text().toStdString().c_str()));
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
		{
			if (treeItem->text() == QString("Config"))
				m_pProConfig->show();
			
			return;
		}

		Item *item = GetItem(treeItem);

		if (!item)
			return;

		item->Show(this);
	}
}

bool ResourceView::ItemsSort(const Item *item1, const Item *item2)
{
	if (item1->GetType() < item2->GetType())
		return true;

	if (item1->GetType() > item2->GetType())
		return false;

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
		case Item::SPRITE:
		{
			while (true)
			{
				name.sprintf("sprite%d", m_lastSpriteID++);

				if (!IsNameExists(name))
					break;
			}
			treeItem = InsertRow(treeItem, name);

			auto item = QSharedPointer<SpriteItem>(new SpriteItem(treeItem, name));
			item->Show(this);
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

			auto item = QSharedPointer<ObjectItem>(new ObjectItem(treeItem, name));
			item->Show(this);
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

			auto item = QSharedPointer<SceneItem>(new SceneItem(treeItem, name));
			item->Show(this);
			m_items.push_back(item);
		} break;
		default: 
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

	item->Show(this);
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
		if (m_items[i]->GetTreeItem() == treeItem)
			m_items.removeAt(i);
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

void ResourceView::RemoveRow(QStandardItem *item) const
{
	if (item->parent() == nullptr)
		return;

	m_pTreeModel->removeRow(item->row(), item->parent()->index());
}

void ResourceView::InsertItem(Item *item)
{
	if (!item)
		return;

	QSharedPointer<Item> ptr(item);

	m_items.push_back(ptr);
}

bool ResourceView::Load(QDataStream *const dataStream, const QString &currPath)
{
	m_pProConfig->Load(dataStream);


	*dataStream >> m_lastSpriteID >> m_lastObjectID >> m_lastSceneID;

	m_mainDir = currPath;

	printf("%d %d %d\n", m_lastSpriteID, m_lastObjectID, m_lastSceneID);

	int size = 0;

	*dataStream >> size;

	printf("Size: %d\n", size);

	for (int i = 0; i < size; ++i)
	{
		int type = 0;
		QString name;

		*dataStream >> type >> name;

		printf("[%d] Type: %d Name: \"%s\"\n", i, type, name.toStdString().c_str());

		QStandardItem *treeItem = m_pTreeModel->item(type);

		treeItem = InsertRow(treeItem, name);

		Item *item = nullptr;

		switch (type)
		{
			case Item::SPRITE:
			{
				item = new SpriteItem(treeItem, name);
			} break;
			case Item::BACKGROUND:
			{

			} break;
			case Item::OBJECT:
			{
				item = new ObjectItem(treeItem, name);
			} break;
			case Item::SCENE:
			{
				item = new SceneItem(treeItem, name);
			} break;
			default: 
			{
				QMessageBox::critical(this, "Error", "Project file is invalid!");
				return false;
			}
		}

		if (!item)
			return false;

		item->Load(dataStream);

		QSharedPointer<Item> sharedItem(item);

		m_items.push_back(sharedItem);
	}

	return true;
}

void ResourceView::Save(QDataStream *const dataStream)
{
	m_pProConfig->Save(dataStream);

	*dataStream << m_lastSpriteID << m_lastObjectID << m_lastSceneID;

	qSort(m_items.begin(), m_items.end(), Item::SavingComparison);

	*dataStream << m_items.size();

	for (auto item : m_items)
	{
		if (item)
			item->Save(dataStream);
	}
}

void ResourceView::GenerateCode(CodeGenerator *codeGenerator)
{
	printf("Generating Canvas...\n");
	codeGenerator->GenerateCanvas(m_pProConfig->GetWndTitle(), m_pProConfig->GetWndSize());

	printf("Generating sprites:\n");
	auto sprites = GetItemsByType(Item::Type::SPRITE);
	for (auto sprite : sprites)
	{
		printf("\t%s...\n", sprite->GetName().toStdString().c_str());
		
		codeGenerator->GenerateSprite(static_cast<SpriteItem*>(sprite));
	}

	printf("Generating objects:\n");
	auto objects = GetItemsByType(Item::Type::OBJECT);
	for (auto object : objects)
	{
		printf("\t%s...\n", object->GetName().toStdString().c_str());

		codeGenerator->GenerateObject(static_cast<ObjectItem*>(object));
	}


	printf("Generating scenes:\n");
	auto scenes = GetItemsByType(Item::Type::SCENE);
	for (auto scene : scenes)
	{
		printf("\t%s...\n", scene->GetName().toStdString().c_str());

		codeGenerator->GenerateScene(static_cast<SceneItem*>(scene));
	}
}

bool ResourceView::IsNameExists(const QString &name)
{
	for (auto item : m_items)
	{
		if (item->GetName() == name)
			return true;
	}

	return false;
}

Item *ResourceView::GetItem(const QStandardItem *treeItem)
{
	for (auto item : m_items)
	{
		if (item->GetTreeItem() == treeItem)
			return item.data();
	}

	return nullptr;
}

Item *ResourceView::GetItem(long long id)
{
	for (auto item : m_items)
	{
		if (item->GetID() == id)
			return item.data();
	}

	return nullptr;
}

QVector<Item*> ResourceView::GetItemsByType(int type)
{
	QVector<Item*> items;

	for (auto item : m_items)
	{
		if (item)
		{
			if (item->GetType() == type)
				items.push_back(item.data());
		}
	}

	return items;
}

void ResourceView::ResourceView_expanded(const QModelIndex &index) const
{
	QStandardItem *item = m_pTreeModel->itemFromIndex(index);

	QIcon icon;
	icon.addFile(":/ResourceView/res/folderOpenIcon.png");

	item->setIcon(icon);
}

void ResourceView::ResourceView_collapsed(const QModelIndex &index) const
{
	QStandardItem *item = m_pTreeModel->itemFromIndex(index);

	QIcon icon;
	icon.addFile(":/ResourceView/res/folderCloseIcon.png");

	item->setIcon(icon);
}
