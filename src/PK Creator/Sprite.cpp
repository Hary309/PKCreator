#include "Sprite.h"

#include <QMessageBox>
#include <QStandardItemModel>

#include <ResourceView.h>
#include <Texture.h>

Sprite::Sprite(QWidget *parent, QStandardItem *item, const QString &itemName)
	: Item(parent, item, itemName)
{
	m_ui.setupUi(this);

	SetName(itemName);

	m_type = Item::SPRITE;

	RefreshTextureBox();

	connect(m_ui.textureBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), this, &Sprite::TextureBox_activated);

	connect(m_ui.okButton, SIGNAL(clicked()), this, SLOT(OkButton_clicked()));
	connect(m_ui.addButton, SIGNAL(clicked()), this, SLOT(AddButton_clicked()));
	connect(m_ui.editButton, SIGNAL(clicked()), this, SLOT(EditButton_clicked()));
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
	m_ui.textureBox->clear();
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
		RefreshTextureBox();
	}

	return false;
}

void Sprite::RefreshTextureBox()
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

	QString currentIndex = m_ui.textureBox->currentText();

	m_ui.textureBox->clear();

	const QVector<Item*> textures = ResourceView::Get()->GetItemsByType(Item::TEXTURE);

	for (int i = 0; i < textures.size(); ++i)
	{
		m_ui.textureBox->insertItem(i, textures[i]->GetName());

		ComboBoxItem *texItem = new ComboBoxItem();

		texItem->index = i;
		texItem->pTex = (Texture*)textures[i];

		m_textures.push_back(texItem);
	}

	m_ui.textureBox->setCurrentIndex(m_ui.textureBox->findText(currentIndex));
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

void Sprite::AddButton_clicked()
{
	QString texName;
	ResourceView *res = ResourceView::Get();
	
	int i = 0;
	
	while (true)
	{
		texName = m_itemName + QString("_tex") + QString::number(i);

		if (!res->IsNameExists(texName))
			break;
	
		i++;
	}

	// texture
	QStandardItem *treeItem = reinterpret_cast<QStandardItemModel*>(res->model())->item(0);
	
	treeItem = res->InsertRow(treeItem, texName);

	Texture *tex = new Texture(res, treeItem, texName);
	tex->show();
	res->InsertItem(tex);

	m_pCurrTex = tex;

	RefreshTextureBox();

	int row = m_ui.textureBox->findText(treeItem->text());

	m_ui.textureBox->setCurrentIndex(row);
}

void Sprite::EditButton_clicked()
{
	if (m_pCurrTex)
		m_pCurrTex->show();
}

void Sprite::TextureBox_activated(int index)
{
	printf("Looking for %d...\n", index);

	for (int i = 0; i < m_textures.size(); ++i)
	{
		if (m_textures[i])
		{
			if (m_textures[i]->index == index)
			{
				printf("New: %d\n", index);

				m_pCurrTex = m_textures[i]->pTex;
				break;
			}
		}
	}
}
