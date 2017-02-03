#pragma once
#include <Item.h>

#include <ui_TextureWindow.h>

class Texture : public Item
{
	Q_OBJECT

private:
	Ui::TextureWindow m_ui;

public:
	Texture(QWidget *parent, QStandardItem *item, const QString &itemName);
	virtual ~Texture();

	virtual void SetName(const QString &name) override;

private slots:
	void OkButton_clicked();
	void LoadButton_clicked();
};

