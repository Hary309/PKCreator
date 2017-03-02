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

	// to save
	Texture					*m_pCurrTex;

	int						m_xCenter;
	int						m_yCenter;

private:
	void RefreshTextureBox();
	void RefreshSpriteCenter();

protected:
	bool event(QEvent *e) override;

public:
	Sprite(QWidget *parent, QStandardItem *item, const QString &itemName);
	~Sprite();

	void SetName(const QString &name) override;
	void Load(QDataStream * const dataStream) override;
	void Save(QDataStream * const dataStream) override;

	Texture *GetTexture() { return m_pCurrTex; }

private slots:
	void OkButton_clicked();
	void AddButton_clicked();
	void EditButton_clicked() const;
	void AutoCenterButton_clicked();
	void TextureBox_activated(int index);
	void CenterXEdit_editingFinished();
	void CenterYEdit_editingFinished();
};

