/*
*********************************************************************
* File          : EventObjectItemWindow.h
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <QMainWindow>
#include <QTimer>

class QCloseEvent;

class EventObjectItem;
class BlueprintEditor;

class EventObjectItemWindow : public QMainWindow
{
	Q_OBJECT

private:
	EventObjectItem		*m_pItemParent;

	BlueprintEditor		*m_pBpEditor;

protected:
	void closeEvent(QCloseEvent *e) override;
	void resizeEvent(QResizeEvent *e) override;

public:
	explicit EventObjectItemWindow(QWidget *parent);
	~EventObjectItemWindow();

	bool FillData(EventObjectItem *item);

};

