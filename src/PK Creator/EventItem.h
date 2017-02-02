#pragma once

#include <QString>

#include <CodeEditorWindow.h>

class QStandardItem;
class QCloseEvent;

/*
Live debugger: 2 sek po ostanim wciœniêtym klawiszu wykonaæ ten kod w wirtualnej maszynie LUA
*/

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

public:
	EventItem(QWidget *parent, Type type, QStandardItem *item);
	~EventItem();

	Type GetType() { return m_type; }
	QStandardItem *GetItem() { return m_item; }

private slots:
	void ActionOk_triggered();
	void ActionSave_triggered();
};

