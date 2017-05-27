/*
*********************************************************************
* File          : ObjectItemWindow.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "ObjectItemWindow.h"

#include <QMessageBox>
#include <QCursor>
#include <QCloseEvent>
#include <QMenu>

#include <ResourceView.h>
#include <EventObjectItem.h>
#include <SpriteItem.h>
#include <ObjectItem.h>
#include <CollisionWindow.h>
#include <LocalVariablesWindow.h>

ObjectItemWindow::ObjectItemWindow(QWidget *parent)
	: ItemWindow(parent)
{
	m_inited = false;

	m_ui.setupUi(this);

	m_pModel = QSharedPointer<QStandardItemModel>(new QStandardItemModel(this));
	m_ui.eventList->setModel(m_pModel.data());

	m_ui.spriteBox->setCurrentIndex(0);

	m_ui.nameEdit->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9]{1,24}")));

	m_varsWindow = nullptr;

	connect(m_ui.okButton, &QPushButton::clicked, this, &ObjectItemWindow::OkButton_clicked);
	connect(m_ui.addButton, &QPushButton::clicked, this, &ObjectItemWindow::AddEventButton_clicked);
	connect(m_ui.removeButton, &QPushButton::clicked, this, &ObjectItemWindow::RemoveEventButton_clicked);
	connect(m_ui.editButton, &QPushButton::clicked, this, &ObjectItemWindow::EditButton_clicked);
	connect(m_ui.eventList, &QListView::doubleClicked, this, &ObjectItemWindow::EditButton_clicked);
	connect(m_ui.addSprButton, &QPushButton::clicked, this, &ObjectItemWindow::AddSprButton_clicked);
	connect(m_ui.editSprButton, &QPushButton::clicked, this, &ObjectItemWindow::EditSprButton_clicked);
	connect(m_ui.spriteBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), this, &ObjectItemWindow::SpriteBox_activated);
	connect(m_ui.varsButton, &QPushButton::clicked, this, &ObjectItemWindow::VarsButton_clicked);
	connect(m_ui.solidBox, &QCheckBox::stateChanged, this, [this](int state) { m_pItemParent->m_solid = state / 2; });
	connect(m_ui.visibleBox, &QCheckBox::stateChanged, this, [this](int state) { m_pItemParent->m_visible = state / 2; });
}

bool ObjectItemWindow::FillData(Item *item)
{
	m_pItemParent = static_cast<ObjectItem*>(item);

	if (!m_pItemParent)
		return false;

	if (m_pItemParent->m_type != Item::OBJECT)
		return false;

	setWindowTitle(item->GetName());

	m_ui.nameEdit->setText(m_pItemParent->GetName());

	if (m_pItemParent->m_solid)
		m_ui.solidBox->setChecked(true);

	if (!m_pItemParent->m_visible)
		m_ui.visibleBox->setChecked(false);

	RefreshSpriteBox();

	bool set = false;

	for (auto spr : m_sprites)
	{
		if (m_pItemParent->m_pCurrSpr == spr->pSpr)
		{
			m_ui.spriteBox->setCurrentIndex(spr->index + 1);
			set = true;
			break;
		}
	}

	if (!set)
		m_ui.spriteBox->setCurrentIndex(0);

	for (auto eventItem : m_pItemParent->m_events)
	{
		auto type = eventItem->GetType();

		QStandardItem *treeItem = nullptr;

		if (type >= EventDefsMgr::COLLISION_EVENT)
		{
			auto collideWith = ResourceView::Get()->GetItem(eventItem->m_idCollideWith);

			if (!collideWith)
				continue;

			treeItem = new QStandardItem("Collision with " + collideWith->GetName() + " Event");
		}
		else
		{
			treeItem = new QStandardItem(EventDefsMgr::Get()->GetEvent(type)->name);
		}

		eventItem->SetItem(treeItem);

		m_pModel->appendRow(treeItem);
	}

	m_varsWindow = QSharedPointer<LocalVariablesWindow>(new LocalVariablesWindow());
	m_varsWindow->SetSource(&m_pItemParent->m_vars);

	CreateContextMenu();

	m_inited = true;

	return true;
}

void ObjectItemWindow::CreateContextMenu()
{
	m_pContextMenu = QSharedPointer<QMenu>(new QMenu("Context menu", this));

	for (auto eventName : *EventDefsMgr::Get()->GetEvents())
	{
		auto action = QSharedPointer<QAction>(new QAction(eventName->name, this));

		m_actionList.push_back(action);
	}

	// Collision
	{
		auto action = QSharedPointer<QAction>(new QAction("Collision event", this));
		m_actionList.push_back(action);
	}

	connect(m_actionList[0].data(), &QAction::triggered, this, [this] { AddEventAction_triggered(0); });
	connect(m_actionList[1].data(), &QAction::triggered, this, [this] { AddEventAction_triggered(1); });
	connect(m_actionList[2].data(), &QAction::triggered, this, [this] { AddEventAction_triggered(2); });
	connect(m_actionList[3].data(), &QAction::triggered, this, [this] { AddEventAction_triggered(3); });
	connect(m_actionList[4].data(), &QAction::triggered, this, [this] { AddEventAction_triggered(4); });
	connect(m_actionList[5].data(), &QAction::triggered, this, [this] { AddEventAction_triggered(5); });
	connect(m_actionList[6].data(), &QAction::triggered, this, [this] { AddEventAction_triggered(6); });
	connect(m_actionList[7].data(), &QAction::triggered, this, [this] { AddEventAction_triggered(7); });
	connect(m_actionList[8].data(), &QAction::triggered, this, [this] { AddEventAction_triggered(8); });
	connect(m_actionList[9].data(), &QAction::triggered, this, [this] { AddCollisionEvent(); });

	for (auto actionItem : m_actionList)
	{
		m_pContextMenu->addAction(actionItem.data());
	}
}

void ObjectItemWindow::RefreshSpriteBox()
{
	m_sprites.clear();

	auto spriteBox = m_ui.spriteBox;

	QString currentIndex = spriteBox->currentText();

	spriteBox->clear();

	spriteBox->insertItem(0, "None");

	auto sprites = ResourceView::Get()->GetItemsByType(Item::SPRITE);

	for (int i = 0; i < sprites.size(); ++i)
	{
		spriteBox->insertItem(i + 1, sprites[i]->GetName());

		auto texItem = QSharedPointer<ComboBoxItem>(new ComboBoxItem());

		texItem->index = i;
		texItem->pSpr = static_cast<SpriteItem*>(sprites[i]);

		m_sprites.push_back(texItem);
	}

	spriteBox->setCurrentIndex(spriteBox->findText(currentIndex));
}

void ObjectItemWindow::changeEvent(QEvent *e)
{
	if (m_inited)
		RefreshSpriteBox();
}

void ObjectItemWindow::closeEvent(QCloseEvent *e)
{
	m_pItemParent->Close();
}

void ObjectItemWindow::AddCollisionEvent()
{
	CollisionWindow colWindow(this);

	colWindow.exec(m_pItemParent);

	ObjectItem *selectedObject = static_cast<ObjectItem*>(colWindow.GetSelectedItem());

	if (!selectedObject)
		return;

	auto events = m_pItemParent->GetEvents();

	for (auto e : *events)
	{
		if (e->m_idCollideWith == selectedObject->GetID())
		{
			QMessageBox::information(this, "PK Creator", "Collision with " + selectedObject->GetName() + " already exists!");
			return;
		}
	}

	int id = 0;

	for (; id <= 100; ++id)
	{
		auto e = m_pItemParent->GetEvent(EventDefsMgr::COLLISION_EVENT + id);

		if (!e)
			break;

		if (id == 100)
			return;
	}

	QStandardItem *item = new QStandardItem("Collision with " + selectedObject->GetName() + " Event");
	item->setEditable(false);
	m_pModel->appendRow(item);

	auto objEvent = QSharedPointer<EventObjectItem>(new EventObjectItem(m_pItemParent, EventDefsMgr::Type(EventDefsMgr::COLLISION_EVENT + id), item));
	objEvent->m_idCurrentObject = m_pItemParent->GetID();
	objEvent->m_idCollideWith = selectedObject->GetID();
	objEvent->SetAsNew();
	objEvent->Show(this);

	m_pItemParent->m_events.push_back(objEvent);
}

void ObjectItemWindow::OkButton_clicked()
{
	QString name = m_ui.nameEdit->text();

	if (ResourceView::Get()->IsNameExists(name) && name != m_pItemParent->m_itemName)
	{
		QMessageBox::information(this, "PK Creator", "This name already exists!");
		return;
	}

	if (m_pItemParent->m_pCurrSpr)
	{
		m_pItemParent->m_pTreeItem->setIcon(QIcon(ResourceView::Get()->GetMainDir() + "\\" + m_pItemParent->m_pCurrSpr->GetTexPath()));
	}

	m_pItemParent->SetName(name);
	m_pItemParent->Close();
}

void ObjectItemWindow::AddEventButton_clicked() const
{
	m_pContextMenu->exec(QCursor::pos());
}

void ObjectItemWindow::RemoveEventButton_clicked()
{
	QModelIndex index = m_ui.eventList->currentIndex();

	auto events = &m_pItemParent->m_events;

	for (int i = 0; i < events->size(); ++i)
	{
		if (events->at(i))
		{
			if (events->at(i)->GetItem() == m_pModel->itemFromIndex(index))
				events->removeAt(i);
		}
	}

	m_pModel->removeRow(index.row());
}

void ObjectItemWindow::EditButton_clicked()
{
	QModelIndex index = m_ui.eventList->currentIndex();

	if (!index.isValid())
		return;

	EventObjectItem *ev = m_pItemParent->GetEvent(m_pModel->itemFromIndex(index));

	ev->Show(this);
}

void ObjectItemWindow::AddEventAction_triggered(int eventType)
{
	if (eventType < 0 && eventType > EventDefsMgr::Get()->GetEvents()->size())
		return;

	if (m_pItemParent->GetEvent(eventType))
	{
		QMessageBox::information(this, "PK Creator", QString::asprintf("%s already exists!", EventDefsMgr::Get()->GetEvent(static_cast<EventDefsMgr::Type>(eventType))->name.toStdString().c_str()));

		return;
	}

	QStandardItem *item = new QStandardItem(EventDefsMgr::Get()->GetEvent(static_cast<EventDefsMgr::Type>(eventType))->name);
	item->setEditable(false);
	m_pModel->appendRow(item);

	auto objEvent = QSharedPointer<EventObjectItem>(new EventObjectItem(m_pItemParent, EventDefsMgr::Type(eventType), item));
	objEvent->SetAsNew();
	objEvent->Show(this);

	m_pItemParent->m_events.push_back(objEvent);
}

void ObjectItemWindow::AddSprButton_clicked()
{
	QString texName;
	ResourceView *res = ResourceView::Get();

	int i = 0;

	while (true)
	{
		texName = m_pItemParent->m_itemName + QString("_spr") + QString::number(i);

		if (!res->IsNameExists(texName))
			break;

		i++;
	}

	QStandardItem *treeItem = reinterpret_cast<QStandardItemModel*>(res->model())->item(0);

	treeItem = res->InsertRow(treeItem, texName);

	SpriteItem *spr = new SpriteItem(treeItem, texName);
	spr->Show(res);
	res->InsertItem(spr);

	m_pItemParent->m_pCurrSpr = spr;

	RefreshSpriteBox();

	int row = m_ui.spriteBox->findText(treeItem->text());

	m_ui.spriteBox->setCurrentIndex(row);
}

void ObjectItemWindow::EditSprButton_clicked() const
{
	if (m_pItemParent->m_pCurrSpr)
		m_pItemParent->m_pCurrSpr->Show(ResourceView::Get());
}

void ObjectItemWindow::SpriteBox_activated(int index)
{
	index--;

	for (auto spr : m_sprites)
	{
		if (spr)
		{
			if (spr->index == index)
			{
				m_pItemParent->m_pCurrSpr = spr->pSpr;
				return;
			}
		}
	}

	m_pItemParent->m_pCurrSpr = nullptr;
}

void ObjectItemWindow::VarsButton_clicked()
{
	m_varsWindow->exec();
}
