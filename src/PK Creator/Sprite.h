#pragma once

#include <ui_SpriteWindow.h>

#include <Item.h>

class Texture;

class Sprite : public Item
{
	Q_OBJECT

private:
	struct ComboBoxItem
	{
		int			index;
		Texture		*pTex;
	};

private:
	Ui::SpriteWindow		m_ui;

	QVector<ComboBoxItem*>	m_textures;

	Texture					*m_pCurrTex;

	// to save
private:
	int						m_xCenter;
	int						m_yCenter;

private:
	void RefreshTextureBox();

protected:
	virtual bool event(QEvent *e) override;

public:
	Sprite(QWidget *parent, QStandardItem *item, const QString &itemName);
	~Sprite();

	virtual void SetName(const QString &name) override;

private slots:
	void OkButton_clicked();
	void AddButton_clicked();
	void EditButton_clicked();
	void TextureBox_activated(int index);
	void CenterXEdit_editingFinished();
	void CenterYEdit_editingFinished();
};

