#pragma once

#include <CodeEditorWindow.h>

class QStandardItem;
class QCloseEvent;

class EventItem;

class EventItemWindow : public CodeEditorWindow
{
	Q_OBJECT

private:
	EventItem		*m_pItemParent;

private:
	void closeEvent(QCloseEvent *e) override;
	void keyPressEvent(QKeyEvent *e) override;

public:
	explicit EventItemWindow(QWidget *parent);
	~EventItemWindow();

	void FillData(EventItem *item);

private slots:
	void ActionOk_triggered() override;
	void ActionSave_triggered() override;
};

