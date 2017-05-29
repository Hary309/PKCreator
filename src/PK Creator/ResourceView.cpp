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
#include <QDebug>

#include <NodesWindow.h>

#include <SpriteItem.h>
#include <BackgroundItem.h>
#include <FontItem.h>
#include <ObjectItem.h>
#include <SceneItem.h>
#include <Config.h>
#include <FunctionDefsMgr.h>
#include <EventDefsMgr.h>
#include <GlobalVariablesWindow.h>

#include <CodeGenerator.h>

#include <QMessageBox>

#include <Windows.h>

ResourceView *ResourceView::s_pInst;

ResourceView::ResourceView(QWidget *parent)
	: QTreeView(parent)
{
	m_defaultModel << QString("Sprites") << QString("Backgrounds") << QString("Font")
				   << QString("Objects") << QString("Scenes");

	Setup();

	m_lastSpriteID = 0;
	m_lastBackgroundID = 0;
	m_lastFontID = 0;
	m_lastObjectID = 0;
	m_lastSceneID = 0;

	m_pFunctionDefsMgr = QSharedPointer<FunctionDefsMgr>(new FunctionDefsMgr());
	m_pFunctionDefsMgr->LoadDefs();

	m_pEventDefsMgr = QSharedPointer<EventDefsMgr>(new EventDefsMgr());

	m_pProConfig = QSharedPointer<Config>(new Config(this));
	m_pGlobalVarsWindow = QSharedPointer<GlobalVariablesWindow>(new GlobalVariablesWindow(this));

	s_pInst = this;

	m_pNodesWindow = QSharedPointer<NodesWindow>(new NodesWindow(this));
	m_pNodesWindow->AddFuncDefs(m_pFunctionDefsMgr->GetFunctionsDef());

	connect(this, &QTreeView::expanded, this, &ResourceView::ResourceView_expanded);
	connect(this, &QTreeView::collapsed, this, &ResourceView::ResourceView_collapsed);
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

	// Config
	{
		QStandardItem *item = new QStandardItem("Config");
		QIcon icon;
		icon.addFile(":/ResourceView/res/configIcon.png");
		item->setIcon(icon);
		item->setEditable(false);
		m_pTreeModel->appendRow(item);
	}


	// Global vars
	{
		QStandardItem *item = new QStandardItem("Global variables");
		QIcon icon;
		icon.addFile(":/ResourceView/res/globalVarsIcon.png");
		item->setIcon(icon);
		item->setEditable(false);
		m_pTreeModel->appendRow(item);
	}
}

void ResourceView::mousePressEvent(QMouseEvent *mouseEvent)
{
	QTreeView::mousePressEvent(mouseEvent);

	if (mouseEvent->button() == Qt::MouseButton::RightButton)
	{
		QModelIndex index = selectionModel()->currentIndex();
		QStandardItem *treeItem = m_pTreeModel->itemFromIndex(index);

		if (treeItem->text() == QString("Config") || treeItem->text() == QString("Global variables"))
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
			else if (treeItem->text() == QString("Global variables"))
				m_pGlobalVarsWindow->show();

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
			while (true)
			{
				name.sprintf("background%d", m_lastBackgroundID++);

				if (!IsNameExists(name))
					break;
			}
			treeItem = InsertRow(treeItem, name);

			auto item = QSharedPointer<BackgroundItem>(new BackgroundItem(treeItem, name));
			item->Show(this);
			m_items.push_back(item);
		} break;
		case Item::FONT:
		{
			while (true)
			{
				name.sprintf("font%d", m_lastFontID++);

				if (!IsNameExists(name))
					break;
			}
			treeItem = InsertRow(treeItem, name);

			auto item = QSharedPointer<FontItem>(new FontItem(treeItem, name));
			item->Show(this);
			m_items.push_back(item);
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

	auto currentItem = GetItem(treeItem);

	if (currentItem && currentItem->GetType() == Item::OBJECT)
	{
		// delete object from scenes
		auto scenes = GetItemsByType(Item::SCENE);

		for (auto scene : scenes)
		{
			if (scene)
				static_cast<SceneItem*>(scene)->RemoveSceneObject(static_cast<ObjectItem*>(currentItem));
		}

		// delete collision event from objects
		auto objects = GetItemsByType(Item::OBJECT);

		for (auto object : objects)
		{
			if (object)
				static_cast<ObjectItem*>(object)->RemoveCollisionWith(currentItem->GetID());
		}
	}



	m_pTreeModel->removeRow(treeItem->row(), treeItem->parent()->index());

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
	m_pGlobalVarsWindow->Load(dataStream);

	*dataStream >> m_lastSpriteID >> m_lastBackgroundID >> m_lastFontID >> m_lastObjectID >> m_lastSceneID;

	m_mainDir = currPath;

	qInfo() << QString().asprintf("Last id: %d %d %d %d %d", m_lastSpriteID, m_lastBackgroundID, m_lastFontID, m_lastObjectID, m_lastSceneID);

	int size = 0;

	*dataStream >> size;

	qInfo() << "Item number:" << QString::number(size);

	for (int i = 0; i < size; ++i)
	{
		int type = 0;
		QString name;

		*dataStream >> type >> name;

		qInfo() << QString("[" + QString::number(i) + "] Type: " + QString::number(type) + " Name: '" + name + "'");

		QStandardItem *treeItem = m_pTreeModel->item(type);

		treeItem = InsertRow(treeItem, name);

		Item *item;

		switch (type)
		{
			case Item::SPRITE:
			{
				item = new SpriteItem(treeItem, name);
			} break;
			case Item::BACKGROUND:
			{
				item = new BackgroundItem(treeItem, name);
			} break;
			case Item::FONT:
			{
				item = new FontItem(treeItem, name);
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
				qCritical() << "Project file is invalid!";
				QMessageBox::critical(this, "Error", "Project file is invalid!");
				return false;
			}
		}

		if (!item)
			return false;

		item->Load(dataStream);

		if (item->GetType() == Item::SPRITE)
		{
			treeItem->setIcon(QIcon(GetMainDir() + "\\" + static_cast<SpriteItem*>(item)->GetTexPath()));
		}
		else if (item->GetType() == Item::OBJECT)
		{
			SpriteItem *spriteItem = static_cast<ObjectItem*>(item)->GetSprite();

			if (spriteItem)
				treeItem->setIcon(QIcon(GetMainDir() + "\\" + spriteItem->GetTexPath()));
		}

		QSharedPointer<Item> sharedItem(item);

		m_items.push_back(sharedItem);
	}

	return true;
}

void ResourceView::Save(QDataStream *const dataStream)
{
	m_pProConfig->Save(dataStream);
	m_pGlobalVarsWindow->Save(dataStream);

	*dataStream << m_lastSpriteID << m_lastBackgroundID << m_lastFontID << m_lastObjectID << m_lastSceneID;

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
	qInfo() << "Generating Canvas...";
	codeGenerator->GenerateCanvas(m_pProConfig->GetWndTitle(), m_pProConfig->GetWndSize());

	qInfo() << "Generating global variables...";
	codeGenerator->GenerateGlobalVars(m_pGlobalVarsWindow.data());

	qInfo() << "Generating font...";
	auto fonts = GetItemsByType(Item::Type::FONT);
	for (auto font : fonts)
	{
		qInfo() << QString(" %1...").arg(font->GetName());

		codeGenerator->GenerateFont(static_cast<FontItem*>(font));
	}

	qInfo() << "Generating sprites:";
	auto sprites = GetItemsByType(Item::Type::SPRITE);
	for (auto sprite : sprites)
	{
		qInfo() << QString(" %1...").arg(sprite->GetName());

		codeGenerator->GenerateSprite(static_cast<SpriteItem*>(sprite));
	}

	qInfo() << "Generating objects:";
	auto objects = GetItemsByType(Item::Type::OBJECT);
	for (auto object : objects)
	{
		qInfo() << QString(" %1...").arg(object->GetName());

		codeGenerator->GenerateObject(static_cast<ObjectItem*>(object));
	}


	qInfo() << "Generating scenes:";
	auto scenes = GetItemsByType(Item::Type::SCENE);
	for (auto scene : scenes)
	{
		qInfo() << QString(" %1...").arg(scene->GetName());

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
