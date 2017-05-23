/*
*********************************************************************
* File          : CollisionWindow.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "CollisionWindow.h"

#include <ResourceView.h>
#include <Item.h>

CollisionWindow::CollisionWindow(QWidget *parent)
	: QDialog(parent)
{
	m_selectedObject = nullptr;

	m_ui.setupUi(this);

	connect(m_ui.objectsWidget, &QListWidget::doubleClicked, this, &CollisionWindow::SelectButton_clicked);
	connect(m_ui.selectButton, &QPushButton::clicked, this, &CollisionWindow::SelectButton_clicked);
	connect(m_ui.cancelButton, &QPushButton::clicked, this, &CollisionWindow::CancelButton_clicked);
}

void CollisionWindow::exec(Item *item)
{
	printf("1\n");

	m_ui.objectsWidget->clear();
	m_objectList.clear();

	printf("2\n");
	auto objects = ResourceView::Get()->GetItemsByType(Item::OBJECT);
	printf("3\n");

	for (auto object : objects)
	{
		printf("4\n");

		if (item == object)
			continue;

		printf("5\n");

		QListWidgetItem *listWidgetItem = new QListWidgetItem(object->GetName());

		auto widgetItem = QSharedPointer<WidgetItem>(new WidgetItem);
		widgetItem->listWidgetItem = listWidgetItem;
		widgetItem->object = object;

		m_objectList.push_back(widgetItem);

		m_ui.objectsWidget->addItem(listWidgetItem);
	}

	QDialog::exec();
}

void CollisionWindow::SelectButton_clicked()
{
	auto selectedItem = m_ui.objectsWidget->selectedItems().first();

	for (auto object : m_objectList)
	{
		if (object->listWidgetItem == selectedItem)
		{
			m_selectedObject = object->object;
			accept();
			break;
		}
	}
}

void CollisionWindow::CancelButton_clicked()
{
	m_selectedObject = nullptr;
	reject();
}
