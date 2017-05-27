/*
*********************************************************************
* File          : SceneItemWindow.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include "ui_SceneWindow.h"

class SceneItem;
class SceneEditor;
class Item;

class BackgroundItem;

#include <ItemWindow.h>

#include <QVector>
#include <QSharedPointer>

namespace sf
{
	class Texture;
	class Sprite;
}

class SceneItemWindow : public ItemWindow
{
	struct ComboBoxItem
	{
		int index;
		BackgroundItem *pBg;
	};

	Q_OBJECT

	struct ObjectListItem
	{
		QListWidgetItem		*listItem;
		long long			id;
	};

private:
	Ui::SceneWindow								m_ui;
	SceneItem									*m_pItemParent;
	QSharedPointer<SceneEditor>					m_pEditor;

	QSharedPointer<sf::Texture>					m_pBgTexture;
	QSharedPointer<sf::Sprite>					m_pBgSprite;

	QVector<QSharedPointer<ObjectListItem>>		m_objectsList;

	QVector<QSharedPointer<ComboBoxItem>>		m_backgrounds;

private:
	void RefreshBgBox();

protected:
	void enterEvent(QEvent *e)		override;
	void closeEvent(QCloseEvent *e) override;
	void showEvent(QShowEvent *e)	override;
	bool event(QEvent *e)			override;

public:
	explicit SceneItemWindow(QWidget *parent);
	virtual ~SceneItemWindow() = default;

	void RefreshObjectList();

	ObjectListItem *GetObjectListItem(QListWidgetItem *item);

	bool FillData(Item *item) override;

	void SetLabelText(const QString &text);

	auto GetBgImage() { return &m_pBgSprite; }

private slots:
	void OkButton_clicked();
	void BgColorButton_clicked();
	void ObjectList_ItemClicked(QListWidgetItem *item);

	void BackgroundBox_activated(int index);

	void snapXBox_valueChanged(int i);
	void snapYBox_valueChanged(int i);

	void DrawGrid_stateChanged(int state);
};

