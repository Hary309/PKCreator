#pragma once
#include <Item.h>

#include <QVector>

#include <ui_ObjectWindow.h>

class EventItem;
class QCloseEvent;

class Object : public Item
{
	Q_OBJECT

private:
	Ui::ObjectWindow		m_ui;

	QStandardItemModel		*m_pModel;

	QVector<EventItem*>		m_events;
	QStringList				m_eventName;

	QMenu					*m_pContextMenu;
	QList<QAction*>			m_actionList;

private:
	void SetupContextMenu();

public:
	Object(QWidget *parent, QStandardItem *item, const QString &itemName);
	virtual ~Object();

	virtual void SetName(const QString &name) override;

	EventItem *GetEvent(int eventType);
	EventItem *GetEvent(QStandardItem *item);

private slots:
	void OkButton_clicked();
	void AddEventButton_clicked();
	void RemoveEventButton_clicked();
	void EditButton_clicked();

	void AddEventAction_triggered(int type);
};

