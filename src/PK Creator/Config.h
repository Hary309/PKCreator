/*
*********************************************************************
* File          : Config.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include "ui_ConfigWindow.h"

class Config : public QDialog
{
	Q_OBJECT

	class ComboBoxItem
	{
	public:
		int		index;
		qint64	id;
	};

private:
	Ui::Config 									m_ui;

	qint64										m_startingScene;

	QVector<QSharedPointer<ComboBoxItem>>		m_scenes;

private:
	void RefreshComboBox();

public:
	explicit Config(QWidget *parent = Q_NULLPTR);
	~Config() = default;

	void Load(QDataStream *const dataStream);
	void Save(QDataStream *const dataStream) const;

	QSize GetWndSize() const;
	QString GetWndTitle();
	qint64 GetStartingSceneId() const { return m_startingScene; }

	void show();

private slots:
	void OkButton_clicked();
	void StartingSceneBox_activated(int index);
};
