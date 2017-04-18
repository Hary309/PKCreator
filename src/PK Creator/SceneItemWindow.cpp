/*
*********************************************************************
* File          : SceneItemWindow.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "SceneItemWindow.h"

#include <QVector>
#include <QMouseEvent>
#include <QMessageBox>
#include <QColorDialog>
#include <QList>

#include <ResourceView.h>
#include <Item.h>
#include <SpriteItem.h>

#include <SceneItem.h>
#include <SceneEditor.h>
#include <ObjectItem.h>
#include <TextureMgr.h>
#include <Config.h>

SceneItemWindow::SceneItemWindow(QWidget* parent)
	: ItemWindow(parent)
{
	m_ui.setupUi(this);
	m_ui.nameEdit->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9]{1,24}")));

	m_pItemParent = nullptr;

	m_pEditor = QSharedPointer<SceneEditor>(new SceneEditor(this));
	m_pEditor->setObjectName(QStringLiteral("widget"));

	m_ui.gridLayout_3->addWidget(m_pEditor.data());

	QSize size = ResourceView::Get()->GetConfig()->GetWndSize();

	int menuViewWidth = 180, statusBarHeight = 20, margin = 16;

	resize(size.height() + menuViewWidth + margin * 2, size.height() + statusBarHeight + margin * 2);

	connect(m_ui.okButton, &QPushButton::clicked, this, &SceneItemWindow::OkButton_clicked);
	connect(m_ui.bgColorButton, &QPushButton::clicked, this, &SceneItemWindow::BgColorButton_clicked);
	connect(m_ui.objectList, &QListWidget::itemClicked, this, &SceneItemWindow::ObjectList_ItemClicked);

	connect(m_ui.snapXBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &SceneItemWindow::snapXBox_valueChanged);
	connect(m_ui.snapYBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &SceneItemWindow::snapYBox_valueChanged);
	connect(m_ui.drawGrid, &QCheckBox::stateChanged, this, &SceneItemWindow::DrawGrid_stateChanged);
}

SceneItemWindow::~SceneItemWindow()
{
}

bool SceneItemWindow::FillData(Item *item)
{
	m_pItemParent = reinterpret_cast<SceneItem*>(item);

	setWindowTitle(item->GetName());

	m_ui.nameEdit->setText(item->GetName());

	RefreshObjectList();

	m_pEditor->SetSource(m_pItemParent);

	// Bit shift deletes alpha
	QColor color(m_pItemParent->m_bgColor >> 8);

	QString newColor = QString::asprintf("background-color: rgb(%d, %d, %d);", color.red(), color.green(), color.blue());
	m_ui.bgColorButton->setStyleSheet(newColor);

	return true;
}

void SceneItemWindow::RefreshObjectList()
{
	if (!m_pEditor)
		return;

	if (!m_pEditor->GetTexMgr())
		return;

	m_ui.objectList->clear();

	m_objectsList.clear();

	QVector<Item*> items = ResourceView::Get()->GetItemsByType(Item::OBJECT);

	for (auto item : items)
	{
		ObjectItem *obj = reinterpret_cast<ObjectItem*>(item);
		QListWidgetItem *listWidgetItem = new QListWidgetItem();

		listWidgetItem->setText(obj->GetName());

		SpriteItem *spr = obj->GetSprite();

		auto listItem = QSharedPointer<ObjectListItem>(new ObjectListItem);

		listItem->listItem = listWidgetItem;
		listItem->id = obj->GetID();

		m_objectsList.push_back(listItem);

		if (spr)
		{
			listWidgetItem->setIcon(QIcon(ResourceView::Get()->GetMainDir() + spr->GetTexPath()));

			m_ui.objectList->addItem(listWidgetItem);
		}
	}
}

SceneItemWindow::ObjectListItem *SceneItemWindow::GetObjectListItem(QListWidgetItem *item)
{
	for (int i = 0; i < m_objectsList.size(); ++i)
	{
		if (m_objectsList[i]->listItem == item)
			return m_objectsList[i].data();
	}

	return nullptr;
}

void SceneItemWindow::enterEvent(QEvent *e)
{
	ItemWindow::enterEvent(e);

	RefreshObjectList();
}

void SceneItemWindow::closeEvent(QCloseEvent *e)
{
	m_pItemParent->Close();
}

void SceneItemWindow::showEvent(QShowEvent *e)
{
	RefreshObjectList();
}

void SceneItemWindow::OkButton_clicked()
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

void SceneItemWindow::BgColorButton_clicked()
{
	QColorDialog colorDialog;

	colorDialog.setCurrentColor(m_pItemParent->m_bgColor >> 8);
	colorDialog.setWindowTitle("Choose background color");

	connect(&colorDialog, &QColorDialog::colorSelected, this, [this](const QColor &color)
	{
		// Bit shift deletes alpha
		m_pItemParent->m_bgColor = (color.rgb() << 8);
		QString newColor = QString::asprintf("background-color: rgb(%d, %d, %d);", color.red(), color.green(), color.blue());
		m_ui.bgColorButton->setStyleSheet(newColor);
	}
	);

	colorDialog.exec();
}

void SceneItemWindow::ObjectList_ItemClicked(QListWidgetItem *item)
{
	ObjectItem *objItem = static_cast<ObjectItem*>(ResourceView::Get()->GetItem(GetObjectListItem(item)->id));

	if (!objItem)
		return;

	if (objItem->GetType() == Item::OBJECT)
	{
		QString path = ResourceView::Get()->GetMainDir() + objItem->GetSprite()->GetTexPath();

		m_pEditor->GetTexMgr()->LoadTexture(objItem->GetSprite());

		m_pEditor->SetCurrObject(objItem);
	}
}

void SceneItemWindow::snapXBox_valueChanged(int i)
{
	m_pEditor->m_snapX = i;
}

void SceneItemWindow::snapYBox_valueChanged(int i)
{
	m_pEditor->m_snapY = i;
}

void SceneItemWindow::DrawGrid_stateChanged(int state)
{
	// state: 0 if unchecked, 2 if checked so this is the fastest way
	m_pEditor->m_drawGrid = state / 2;
}
