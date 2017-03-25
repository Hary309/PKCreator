#pragma once

#include <QDialog>

class QStandardItem;
class QCloseEvent;

class EventItem;

class EventItemWindow : public QDialog
{
	Q_OBJECT

private:
	EventItem		*m_pItemParent;

private:
	void closeEvent(QCloseEvent *e) override;

public:
	explicit EventItemWindow(QWidget *parent);
	~EventItemWindow();

	void FillData(EventItem *item);
};

