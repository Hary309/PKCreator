#pragma once
#include <QDialog>

class Item;

class ItemWindow : public QDialog
{
protected:
	Item		*m_pItemParent;

public:
	explicit ItemWindow(QWidget *parent) : QDialog(parent)
	{
		setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
		m_pItemParent = nullptr;
	}

	virtual ~ItemWindow() {  }

	virtual bool FillData(Item *item) = 0;
};

