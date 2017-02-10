#pragma once
#include <Item.h>

#include <ui_TextureWindow.h>

class Texture : public Item
{
	Q_OBJECT

private:
	Ui::TextureWindow	m_ui;

private: // to save
	int					m_width;
	int					m_height;

public:
	Texture(QWidget *parent, QStandardItem *item, const QString &itemName);
	virtual ~Texture();

	virtual void SetName(const QString &name) override;

	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }

	virtual void Load(QDataStream *const dataStream) override;
	virtual void Save(QDataStream *const dataStream) override;

private slots:
	void OkButton_clicked();
	void LoadButton_clicked();
};

