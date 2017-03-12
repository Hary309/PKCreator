#pragma once
#include <Item.h>

#include <QVector>

class EventItem;
class SpriteItem;
class ObjectItemWindow;

class ObjectItem : public Item
{
protected:
	SpriteItem				*m_pCurrSpr;
	QVector<EventItem*>		m_events;

	QStringList				m_eventName;

	friend					ObjectItemWindow;

public:
	ObjectItem(QStandardItem *item, const QString &itemName);
	virtual ~ObjectItem();

	void SetName(const QString &name) override;

	EventItem *GetEvent(int eventType);
	EventItem *GetEvent(QStandardItem *item);

	SpriteItem *GetSprite() const { return m_pCurrSpr; }

	void Load(QDataStream *const dataStream) override;
	void Save(QDataStream *const dataStream) override;

	void Show(QWidget *wndParent)				override;
	void Close()								override;
};
