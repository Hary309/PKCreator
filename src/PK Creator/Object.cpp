#include "Object.h"

#include <QMessageBox>
#include <QMenu>
#include <QCursor>
#include <QCloseEvent>

#include <ResourceView.h>
#include <EventItem.h>

Object::Object(QWidget *parent, QStandardItem *item, const QString &itemName)
	: Item(parent, item, itemName)
{
	m_ui.setupUi(this);

	SetName(itemName);

	m_type = Item::OBJECT;

	m_pModel = new QStandardItemModel(this);
	m_ui.eventList->setModel(m_pModel);

	m_eventName << "Create Event" << "Destroy Event" << "Pulse Event" << "Render Event";

	SetupContextMenu();
	RefreshSpriteBox();

	m_ui.spriteBox->setCurrentIndex(0);

	connect(m_ui.okButton, SIGNAL(clicked()), this, SLOT(OkButton_clicked()));
	connect(m_ui.addButton, SIGNAL(clicked()), this, SLOT(AddEventButton_clicked()));
	connect(m_ui.removeButton, SIGNAL(clicked()), this, SLOT(RemoveEventButton_clicked()));
	connect(m_ui.editButton, SIGNAL(clicked()), this, SLOT(EditButton_clicked()));
	connect(m_ui.eventList, &QListView::doubleClicked, this, &Object::EditButton_clicked);
}

Object::~Object()
{
	m_pModel->clear();

	if (m_pModel)
	{
		delete m_pModel;
		m_pModel = nullptr;
	}

	for (int i = 0; i < m_events.size(); ++i)
	{
		if (m_events[i])
		{
			delete m_events[i];
			m_events[i] = nullptr;
		}
	}

	m_events.clear();

	for (int i = 0; i < m_actionList.size(); ++i)
	{
		if (m_actionList[i])
		{
			delete m_actionList[i];
			m_actionList[i] = nullptr;
		}
	}

	m_actionList.clear();

	if (m_pContextMenu)
	{
		delete m_pContextMenu;
		m_pContextMenu = nullptr;
	}

	for (int i = 0; i < m_sprites.size(); ++i)
	{
		if (m_sprites[i])
		{
			delete m_sprites[i];
			m_sprites[i] = nullptr;
		}
	}

	m_sprites.clear();
	m_ui.spriteBox->clear();
}

void Object::SetupContextMenu()
{
	m_pContextMenu = new QMenu("Context menu", this);

	for (int i = 0; i < m_eventName.size(); ++i)
	{
		QAction *action = new QAction(m_eventName[i], this);

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

void Object::RefreshSpriteBox()
{
	for (int i = 0; i < m_sprites.size(); ++i)
	{
		if (m_sprites[i])
		{
			delete m_sprites[i];
			m_sprites[i] = nullptr;
		}
	}

	m_sprites.clear();

	QString currentIndex = m_ui.spriteBox->currentText();

	m_ui.spriteBox->clear();

	m_ui.spriteBox->insertItem(0, "None");

	const QVector<Item*> sprites = ResourceView::Get()->GetItemsByType(Item::SPRITE);

	for (int i = 0; i < sprites.size(); ++i)
	{
		m_ui.spriteBox->insertItem(i + 1, sprites[i]->GetName());

		ComboBoxItem *texItem = new ComboBoxItem();

		texItem->index = i;
		texItem->pSpr = (Sprite*)sprites[i];

		m_sprites.push_back(texItem);
	}

	m_ui.spriteBox->setCurrentIndex(m_ui.spriteBox->findText(currentIndex));
}

void Object::SetName(const QString &name)
{
	m_itemName = name;
	setWindowTitle(name);
	m_ui.nameEdit->setText(name);
	m_pItem->setText(name);
}

bool Object::event(QEvent * e)
{
	Item::event(e);

	if (e->type() == QEvent::WindowActivate)
	{
		RefreshSpriteBox();

		printf("refresh\n");
	}

	return false;
}

EventItem *Object::GetEvent(int eventType)
{
	for (int i = 0; i < m_events.size(); ++i)
	{
		if (m_events[i]->GetType() == eventType)
			return m_events[i];
	}

	return nullptr;
}

EventItem * Object::GetEvent(QStandardItem *item)
{
	for (int i = 0; i < m_events.size(); ++i)
	{
		if (m_events[i]->GetItem() == item)
			return m_events[i];
	}

	return nullptr;
}

void Object::OkButton_clicked()
{
	QString name = m_ui.nameEdit->text();

	if (ResourceView::Get()->IsNameExists(name) && name != m_itemName)
	{
		QMessageBox::information(this, "PK Creator", "This name already exists!");
		return;
	}

	SetName(name);
	hide();
}

void Object::AddEventButton_clicked()
{
	m_pContextMenu->exec(QCursor::pos());
}

void Object::RemoveEventButton_clicked()
{
	QModelIndex index = m_ui.eventList->currentIndex();

	for (int i = 0; i < m_events.size(); ++i)
	{
		if (m_events[i])
		{
			if (m_events[i]->GetItem() == m_pModel->itemFromIndex(index))
			{
				delete m_events[i];
				m_events[i] = nullptr;

				m_events.removeAt(i);
			}
		}
	}

	m_pModel->removeRow(index.row());
}

void Object::EditButton_clicked()
{
	QModelIndex index = m_ui.eventList->currentIndex();

	if (!index.isValid())
		return;

	EventItem *ev = GetEvent(m_pModel->itemFromIndex(index));

	ev->show();
}

void Object::AddEventAction_triggered(int eventType)
{
	if (eventType < 0 && eventType > m_eventName.size())
		return;

	if (GetEvent(eventType))
	{
		QMessageBox::information(this, "PK Creator", QString::asprintf("%s already exists!", m_eventName[eventType].toStdString().c_str()));

		return;
	}

	QStandardItem *item = new QStandardItem(m_eventName[eventType]);
	item->setEditable(false);
	m_pModel->appendRow(item);

	EventItem *objEvent = new EventItem(this, (EventItem::Type)eventType, item);
	objEvent->setWindowTitle(QString::asprintf("%s - %s", windowTitle().toStdString().c_str(), m_eventName[eventType].toStdString().c_str()));
	objEvent->show();

	m_events.push_back(objEvent);
}
