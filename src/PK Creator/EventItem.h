#pragma once

#include <QString>

#include <CodeEditorWindow.h>

class QStandardItem;
class QCloseEvent;

class EventItem : public CodeEditorWindow
{
	Q_OBJECT

public:
	enum Type
	{
		CREATE = 0,
		DESTROY,
		PULSE,
		RENDER
	};

private:
	Type			m_type;
	QString			m_code;

	QStandardItem	*m_item;

private:
	void closeEvent(QCloseEvent *closeEvent);
	void keyPressEvent(QKeyEvent * e);

public:
	EventItem(QWidget *parent, Type type, QStandardItem *item);
	~EventItem();

	Type GetType() { return m_type; }
	QStandardItem *GetItem() { return m_item; }

	void Load(QDataStream * const dataStream);
	void Save(QDataStream * const dataStream);

private slots:
	void ActionOk_triggered();
	void ActionSave_triggered();
};

