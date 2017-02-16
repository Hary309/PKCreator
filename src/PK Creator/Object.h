#pragma once
#include <Item.h>

#include <QVector>

#include <ui_ObjectWindow.h>

class EventItem;
class QCloseEvent;
class Sprite;

class Object : public Item
{
	Q_OBJECT

private:
	struct ComboBoxItem
	{
		int index;
		Sprite *pSpr;
	};

private:
	Ui::ObjectWindow		m_ui;

	QStandardItemModel		*m_pModel;

	QStringList				m_eventName;

	QMenu					*m_pContextMenu;
	QList<QAction*>			m_actionList;

	QVector<ComboBoxItem*>	m_sprites;

	// to save
	Sprite					*m_pCurrSpr;
	QVector<EventItem*>		m_events;

private:
	void SetupContextMenu();
	void RefreshSpriteBox();

protected:
	virtual bool event(QEvent *e) override;

public:
	Object(QWidget *parent, QStandardItem *item, const QString &itemName);
	virtual ~Object();

	virtual void SetName(const QString &name) override;

	EventItem *GetEvent(int eventType);
	EventItem *GetEvent(QStandardItem *item);

	Sprite *GetSprite() { return m_pCurrSpr; }

	virtual void Load(QDataStream *const dataStream) override;
	virtual void Save(QDataStream *const dataStream) override;


private slots:
	void OkButton_clicked();
	void AddEventButton_clicked();
	void RemoveEventButton_clicked();
	void EditButton_clicked();
	void AddEventAction_triggered(int type);
	void AddSprButton_clicked();
	void EditSprButton_clicked();
	void SpriteBox_activated(int index);
};

