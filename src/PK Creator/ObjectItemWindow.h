/*
*********************************************************************
* File          : ObjectItemWindow.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <ui_ObjectWindow.h>

#include <ItemWindow.h>

#include <QVector>
#include <QSharedPointer>

class SpriteItem;
class ObjectItem;
class QStandardItemModel;

class ObjectItemWindow : public ItemWindow
{
	struct ComboBoxItem
	{
		int index;
		SpriteItem *pSpr;
	};

	Q_OBJECT

private:
	Ui::ObjectWindow						m_ui;

	ObjectItem								*m_pItemParent;

	QSharedPointer<QStandardItemModel>		m_pModel;

	QSharedPointer<QMenu>					m_pContextMenu;
	QVector<QSharedPointer<QAction>>		m_actionList;

	QVector<QSharedPointer<ComboBoxItem>>	m_sprites;

	bool									m_inited;

private:
	void CreateContextMenu();
	void RefreshSpriteBox();

protected:
	void changeEvent(QEvent *e) override;
	void closeEvent(QCloseEvent *event) override;

public:
	explicit ObjectItemWindow(QWidget *parent);
	virtual ~ObjectItemWindow();

	bool FillData(Item *item) override;

private slots:
	void OkButton_clicked();
	void AddEventButton_clicked() const;
	void RemoveEventButton_clicked();
	void EditButton_clicked();
	void AddEventAction_triggered(int type);
	void AddSprButton_clicked();
	void EditSprButton_clicked() const;
	void SpriteBox_activated(int index);

};

