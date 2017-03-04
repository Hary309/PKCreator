#pragma once

#include <ui_SpriteWindow.h>

#include <Item.h>

class Sprite : public Item
{
	Q_OBJECT

private:
	Ui::SpriteWindow		m_ui;

	QSize					m_size;

	QString					m_texPath; // relative path ("resources/m_itemName.ext")

	QPoint					m_center;

public:
	Sprite(QWidget *parent, QStandardItem *item, const QString &itemName);
	~Sprite();

	QSize GetSize()			const	 { return m_size; }
	QPoint GetCenter()		const	 { return m_center; }
	QString GetTexPath()	const	 { return m_texPath; }

	void SetName(const QString &name)		  override;
	void Load(QDataStream * const dataStream) override;
	void Save(QDataStream * const dataStream) override;

private slots:
	void OkButton_clicked();
	void CenterButton_clicked();
	void LoadSpriteButton_clicked();

	void CenterXEdit_editingFinished();
	void CenterYEdit_editingFinished();
};

