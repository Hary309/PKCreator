#include "Object.h"

#include <QMessageBox>
#include <QMenu>
#include <QCursor>
#include <QCloseEvent>

#include <ResourceView.h>
#include <EventItem.h>
#include <Sprite.h>

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

	m_pCurrSpr = nullptr;

	m_ui.spriteBox->setCurrentIndex(0);

	m_ui.nameEdit->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9]{1,24}")));

	connect(m_ui.okButton, SIGNAL(clicked()), this, SLOT(OkButton_clicked()));
	connect(m_ui.addButton, SIGNAL(clicked()), this, SLOT(AddEventButton_clicked()));
	connect(m_ui.removeButton, SIGNAL(clicked()), this, SLOT(RemoveEventButton_clicked()));
	connect(m_ui.editButton, SIGNAL(clicked()), this, SLOT(EditButton_clicked()));
	connect(m_ui.eventList, &QListView::doubleClicked, this, &Object::EditButton_clicked);
	connect(m_ui.addSprButton, SIGNAL(clicked()), this, SLOT(AddSprButton_clicked()));
	connect(m_ui.editSprButton, SIGNAL(clicked()), this, SLOT(EditSprButton_clicked()));
	connect(m_ui.spriteBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), this, &Object::SpriteBox_activated);
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

void Object::AddSprButton_clicked()
{
	QString texName;
	ResourceView *res = ResourceView::Get();

	int i = 0;

	while (true)
	{
		texName = m_itemName + QString("_spr") + QString::number(i);

		if (!res->IsNameExists(texName))
			break;

		i++;
	}

	// texture
	QStandardItem *treeItem = reinterpret_cast<QStandardItemModel*>(res->model())->item(1);

	treeItem = res->InsertRow(treeItem, texName);

	Sprite *spr = new Sprite(res, treeItem, texName);
	spr->show();
	res->InsertItem(spr);

	m_pCurrSpr = spr;

	RefreshSpriteBox();

	int row = m_ui.spriteBox->findText(treeItem->text());

	m_ui.spriteBox->setCurrentIndex(row);
}

void Object::EditSprButton_clicked()
{
	if (m_pCurrSpr)
		m_pCurrSpr->show();
}

void Object::SpriteBox_activated(int index)
{
	index--;

	for (int i = 0; i < m_sprites.size(); ++i)
	{
		if (m_sprites[i])
		{
			if (m_sprites[i]->index == index)
			{
				m_pCurrSpr = m_sprites[i]->pSpr;
				return;
			}
		}
	}

	m_pCurrSpr = nullptr;
}

void Object::Load(QDataStream *const dataStream)
{
	QString name;

	*dataStream >> name;

	printf("Name: \"%s\"\n", name.toStdString().c_str());

	m_pCurrSpr = (Sprite*)ResourceView::Get()->GetItem(name);
	RefreshSpriteBox();

	for (int i = 0; i < m_sprites.size(); ++i)
	{
		if (m_pCurrSpr == m_sprites[i]->pSpr)
		{
			m_ui.spriteBox->setCurrentIndex(m_sprites[i]->index + 1);

			printf("%s\n", m_ui.spriteBox->currentText().toStdString().c_str());
		}
	}

	int size;
	*dataStream >> size;

	printf("Size: %d\n", size);

	for (int i = 0; i < size; ++i)
	{
		int type = 0;

		*dataStream >> type;

		printf("Type: %d\n", type);

		QStandardItem *item = new QStandardItem(m_eventName[type]);
		item->setEditable(false);
		m_pModel->appendRow(item);

		EventItem *eventItem = new EventItem(this, (EventItem::Type)type, item);
		eventItem->setWindowTitle(QString::asprintf("%s - %s", windowTitle().toStdString().c_str(), m_eventName[type].toStdString().c_str()));
		eventItem->Load(dataStream);

		m_events.push_back(eventItem);
	}
}

void Object::Save(QDataStream *const dataStream)
{
	Item::Save(dataStream);

	*dataStream << (m_pCurrSpr ? m_pCurrSpr->GetName() : QString("")) << m_events.size();

	for (int i = 0; i < m_events.size(); ++i)
	{
		if (m_events[i])
		{
			m_events[i]->Save(dataStream);
		}
	}
}
