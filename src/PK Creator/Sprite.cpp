#include "Sprite.h"

#include <QMessageBox>
#include <QStandardItemModel>

#include <ResourceView.h>

Sprite::Sprite(QWidget *parent, QStandardItem *item, const QString &itemName)
	: Item(parent, item, itemName)
{
	m_ui.setupUi(this);

	SetName(itemName);

	m_type = Item::SPRITE;

	RefreshSpriteBox();

	connect(m_ui.okButton, SIGNAL(clicked()), this, SLOT(OkButton_clicked()));
}


Sprite::~Sprite()
{
	for (int i = 0; i < m_textures.size(); ++i)
	{
		if (m_textures[i])
		{
			delete m_textures[i];
			m_textures[i] = nullptr;
		}
	}

	m_textures.clear();
	m_ui.spriteBox->clear();
}

void Sprite::SetName(const QString & name)
{
	m_itemName = name;
	setWindowTitle(name);
	m_ui.nameEdit->setText(name);
	m_pItem->setText(name);
}

bool Sprite::event(QEvent *e)
{
	Item::event(e);
	
	if (e->type() == QEvent::WindowActivate)
	{
		RefreshSpriteBox();
	}

	return false;
}

void Sprite::RefreshSpriteBox()
{
	for (int i = 0; i < m_textures.size(); ++i)
	{
		if (m_textures[i])
		{
			delete m_textures[i];
			m_textures[i] = nullptr;
		}
	}

	m_textures.clear();

	QString currentIndex = m_ui.spriteBox->currentText();

	m_ui.spriteBox->clear();

	const QVector<Item*> textures = ResourceView::Get()->GetItemsByType(Item::TEXTURE);

	for (int i = 0; i < textures.size(); ++i)
	{
		m_ui.spriteBox->insertItem(i, textures[i]->GetName());

		TexItem *texItem = new TexItem();

		texItem->index = i;
		texItem->pTex = (Texture*)textures[i];

		m_textures.push_back(texItem);
	}

	m_ui.spriteBox->setCurrentIndex(m_ui.spriteBox->findText(currentIndex));
}

void Sprite::OkButton_clicked()
{
	QString name = m_ui.nameEdit->text();

	if (ResourceView::Get()->IsNameExists(name) && name != m_itemName)
	{
		QMessageBox::information(this, "PK Creator", "This name already exists!");
		return;
	}

	SetName(name);
	hide();
}
