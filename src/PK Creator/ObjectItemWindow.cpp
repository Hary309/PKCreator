#include "ObjectItemWindow.h"

#include <QMessageBox>
#include <QCursor>
#include <QCloseEvent>

#include <ResourceView.h>
#include <EventItem.h>
#include <SpriteItem.h>
#include <ObjectItem.h>

ObjectItemWindow::ObjectItemWindow(QWidget *parent)
	: ItemWindow(parent)
{
	m_inited = false;

	m_ui.setupUi(this);

	m_pModel = new QStandardItemModel(this);
	m_ui.eventList->setModel(m_pModel);

	m_ui.spriteBox->setCurrentIndex(0);

	m_ui.nameEdit->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9]{1,24}")));

	connect(m_ui.okButton, SIGNAL(clicked()), this, SLOT(OkButton_clicked()));
	connect(m_ui.addButton, SIGNAL(clicked()), this, SLOT(AddEventButton_clicked()));
	connect(m_ui.removeButton, SIGNAL(clicked()), this, SLOT(RemoveEventButton_clicked()));
	connect(m_ui.editButton, SIGNAL(clicked()), this, SLOT(EditButton_clicked()));
	connect(m_ui.eventList, &QListView::doubleClicked, this, &ObjectItemWindow::EditButton_clicked);
	connect(m_ui.addSprButton, SIGNAL(clicked()), this, SLOT(AddSprButton_clicked()));
	connect(m_ui.editSprButton, SIGNAL(clicked()), this, SLOT(EditSprButton_clicked()));
	connect(m_ui.spriteBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), this, &ObjectItemWindow::SpriteBox_activated);
}

ObjectItemWindow::~ObjectItemWindow()
{
	m_pModel->clear();

	if (m_pModel)
	{
		delete m_pModel;
		m_pModel = nullptr;
	}

	qDeleteAll(m_actionList);

	if (m_pContextMenu)
	{
		delete m_pContextMenu;
		m_pContextMenu = nullptr;
	}

	qDeleteAll(m_sprites);

	m_ui.spriteBox->clear();
}

bool ObjectItemWindow::FillData(Item* item)
{
	m_pItemParent = static_cast<ObjectItem*>(item);

	if (!m_pItemParent)
		return false;

	if (m_pItemParent->m_type != Item::OBJECT)
		return false;

	setWindowTitle(item->GetName());

	m_ui.nameEdit->setText(m_pItemParent->GetName());

	RefreshSpriteBox();

	bool set = false;

	for (int i = 0; i < m_sprites.size(); ++i)
	{
		if (m_pItemParent->m_pCurrSpr == m_sprites[i]->pSpr)
		{
			m_ui.spriteBox->setCurrentIndex(m_sprites[i]->index + 1);
			set = true;
			break;
		}
	}

	if (!set)
		m_ui.spriteBox->setCurrentIndex(0);

	for (auto eventItem : m_pItemParent->m_events)
	{
		QStandardItem *item = new QStandardItem(m_pItemParent->m_eventName[eventItem->GetType()]);

		eventItem->SetItem(item);

		m_pModel->appendRow(item);
	}

	CreateContextMenu();

	m_inited = true;

	return true;
}

void ObjectItemWindow::CreateContextMenu()
{
	m_pContextMenu = new QMenu("Context menu", this);

	for (int i = 0; i < m_pItemParent->m_eventName.size(); ++i)
	{
		QAction *action = new QAction(m_pItemParent->m_eventName[i], this);

		m_actionList.push_back(action);
	}

	connect(m_actionList[0], &QAction::triggered, this, [this] { AddEventAction_triggered(0); });
	connect(m_actionList[1], &QAction::triggered, this, [this] { AddEventAction_triggered(1); });
	connect(m_actionList[2], &QAction::triggered, this, [this] { AddEventAction_triggered(2); });
	connect(m_actionList[3], &QAction::triggered, this, [this] { AddEventAction_triggered(3); });

	for (int i = 0; i < m_actionList.size(); ++i)
	{
		m_pContextMenu->addAction(m_actionList[i]);
	}
}

void ObjectItemWindow::RefreshSpriteBox()
{
	for (int i = 0; i < m_sprites.size(); ++i)
	{
		if (m_sprites[i])
			delete m_sprites[i];
	}

	m_sprites.clear();

	QString currentIndex = m_ui.spriteBox->currentText();

	m_ui.spriteBox->clear();

	m_ui.spriteBox->insertItem(0, "None");

	const QVector<Item*> sprites = ResourceView::Get()->GetItemsByType(Item::SPRITE);

	printf("Size: %d\n", sprites.size());

	for (int i = 0; i < sprites.size(); ++i)
	{
		m_ui.spriteBox->insertItem(i + 1, sprites[i]->GetName());

		ComboBoxItem *texItem = new ComboBoxItem();

		texItem->index = i;
		texItem->pSpr = static_cast<SpriteItem*>(sprites[i]);

		m_sprites.push_back(texItem);
	}

	m_ui.spriteBox->setCurrentIndex(m_ui.spriteBox->findText(currentIndex));
}

void ObjectItemWindow::changeEvent(QEvent *e)
{
	if (m_inited)
		RefreshSpriteBox();
}

void ObjectItemWindow::closeEvent(QCloseEvent* event)
{
	m_pItemParent->Close();
}

void ObjectItemWindow::OkButton_clicked()
{
	QString name = m_ui.nameEdit->text();

	if (ResourceView::Get()->IsNameExists(name) && name != m_pItemParent->m_itemName)
	{
		QMessageBox::information(this, "PK Creator", "This name already exists!");
		return;
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

	QVector<EventItem*> *events = &m_pItemParent->m_events;

	for (int i = 0; i < events->size(); ++i)
	{
		if (events->at(i))
		{
			if (events->at(i)->GetItem() == m_pModel->itemFromIndex(index))
			{
				delete events->at(i);
				
				events->removeAt(i);
			}
		}
	}

	m_pModel->removeRow(index.row());
}

void ObjectItemWindow::EditButton_clicked()
{
	QModelIndex index = m_ui.eventList->currentIndex();

	if (!index.isValid())
		return;

	EventItem *ev = m_pItemParent->GetEvent(m_pModel->itemFromIndex(index));

	ev->Show(this);
}

void ObjectItemWindow::AddEventAction_triggered(int eventType)
{
	if (eventType < 0 && eventType > m_pItemParent->m_eventName.size())
		return;

	if (m_pItemParent->GetEvent(eventType))
	{
		QMessageBox::information(this, "PK Creator", QString::asprintf("%s already exists!", m_pItemParent->m_eventName[eventType].toStdString().c_str()));

		return;
	}

	QStandardItem *item = new QStandardItem(m_pItemParent->m_eventName[eventType]);
	item->setEditable(false);
	m_pModel->appendRow(item);

	EventItem *objEvent = new EventItem(EventItem::Type(eventType), item);
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

	for (int i = 0; i < m_sprites.size(); ++i)
	{
		if (m_sprites[i])
		{
			if (m_sprites[i]->index == index)
			{
				m_pItemParent->m_pCurrSpr = m_sprites[i]->pSpr;
				return;
			}
		}
	}

	m_pItemParent->m_pCurrSpr = nullptr;
}