/*
*********************************************************************
* File          : Config.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "Config.h"

#include <QDebug>

#include <ResourceView.h>
#include <Item.h>

Config::Config(QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);

	if (parent)
		m_ui.titleEdit->setText("Game");

	m_ui.widthEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]{2,4}")));
	m_ui.heightEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]{2,4}")));

	m_ui.widthEdit->setText("800");
	m_ui.heightEdit->setText("600");

	m_startingScene = -1;

	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	connect(m_ui.okButton, &QPushButton::clicked, this, [this] { this->hide(); });
	connect(m_ui.startingSceneBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), this, &Config::StartingSceneBox_activated);
}

void Config::RefreshComboBox()
{
	m_scenes.clear();

	auto sceneBox = m_ui.startingSceneBox;
	sceneBox->clear();

	auto scenes = ResourceView::Get()->GetItemsByType(Item::SCENE);

	for (int i = 0; i < scenes.size(); ++i)
	{
		sceneBox->insertItem(i, scenes[i]->GetName());

		auto texItem = QSharedPointer<ComboBoxItem>(new ComboBoxItem());

		texItem->index = i;
		texItem->id = scenes[i]->GetID();

		m_scenes.push_back(texItem);
	}

	bool set = false;

	for (auto scene : m_scenes)
	{
		if (scene->id == m_startingScene)
		{
			sceneBox->setCurrentIndex(scene->index);

			set = true;
		}
	}

	if (!set)
	{
		if (m_scenes.size() > 0)
		{
			sceneBox->setCurrentIndex(m_scenes[0]->index);
			m_startingScene = m_scenes[0]->id;
		}
	}
}

void Config::Load(QDataStream *const dataStream)
{
	QString title, width, height;

	*dataStream >> title >> width >> height >> m_startingScene;

	qInfo() << QString("Window info: Title:'" + title + "' Size:"+QString::number(width.toInt())+"x"+ QString::number(height.toInt()) +"\n");

	m_ui.titleEdit->setText(title);
	m_ui.widthEdit->setText(width);
	m_ui.heightEdit->setText(height);
}

void Config::Save(QDataStream *const dataStream) const
{
	*dataStream << m_ui.titleEdit->text() << m_ui.widthEdit->text() << m_ui.heightEdit->text() << m_startingScene;
}

QSize Config::GetWndSize() const
{
	QSize size;
	size.setWidth(m_ui.widthEdit->text().toInt());
	size.setHeight(m_ui.heightEdit->text().toInt());

	return size;
}

QString Config::GetWndTitle()
{
	return m_ui.titleEdit->text();
}

void Config::show()
{
	QDialog::show();
	RefreshComboBox();
}

void Config::StartingSceneBox_activated(int index)
{
	for (auto scene : m_scenes)
	{
		if (scene && scene->index == index)
		{
			m_startingScene = scene->id;
			return;
		}
	}

	m_startingScene = -1;
}

void Config::OkButton_clicked()
{
	hide();
	m_scenes.clear();
}
