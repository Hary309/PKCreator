#pragma once

#include <ui_SpriteWindow.h>

#include <Item.h>

class Texture;

class Sprite : public Item
{
	Q_OBJECT

private:
	struct TexItem
	{
		int			index;
		Texture		*pTex;
	};

private:
	Ui::SpriteWindow	m_ui;

	QVector<TexItem*>	m_textures;

private:
	void RefreshSpriteBox();

public:
	Sprite(QWidget *parent, QStandardItem *item, const QString &itemName);
	~Sprite();

	virtual void SetName(const QString &name) override;
	virtual bool event(QEvent *e) override;

private slots:
	void OkButton_clicked();
};

