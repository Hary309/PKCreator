#include "Sprite.h"

#include <QMessageBox>
#include <QStandardItemModel>

#include <ResourceView.h>
#include <Texture.h>

Sprite::Sprite(QWidget *parent, QStandardItem *item, const QString &itemName)
	: Item(parent, item, itemName)
{
	m_ui.setupUi(this);

	m_ui.centerXEdit->setValidator(new QIntValidator);
	m_ui.centerYEdit->setValidator(new QIntValidator);

	SetName(itemName);

	m_type = Item::SPRITE;

	RefreshTextureBox();
	
	m_xCenter = -1;
	m_yCenter = -1;

	m_pCurrTex = nullptr;

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
		RefreshSpriteCenter();
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

	QStandardItem *treeItem = reinterpret_cast<QStandardItemModel*>(res->model())->item(0);
	
	treeItem = res->InsertRow(treeItem, texName);

	Texture *tex = new Texture(res, treeItem, texName);
	tex->show();
	res->InsertItem(tex);

	connect(tex, &QDialog::accepted, this, [this] { RefreshSpriteCenter(); });

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
	for (int i = 0; i < m_textures.size(); ++i)
	{
		if (m_textures[i])
		{
			if (m_textures[i]->index == index)
			{
				m_pCurrTex = m_textures[i]->pTex;

				RefreshSpriteCenter();
				break;
			}
		}
	}
}

void Sprite::CenterXEdit_editingFinished()
{
	 m_xCenter = m_ui.centerXEdit->text().toInt();
}

void Sprite::CenterYEdit_editingFinished()
{
	m_yCenter = m_ui.centerYEdit->text().toInt();
}

void Sprite::RefreshSpriteCenter()
{
	if (!m_pCurrTex)
		return;

	if (m_xCenter == -1 || m_yCenter == -1)
	{
		m_xCenter = m_pCurrTex->GetWidth() / 2;
		m_yCenter = m_pCurrTex->GetHeight() / 2;

		m_ui.centerXEdit->setText(QString::number(m_xCenter));
		m_ui.centerYEdit->setText(QString::number(m_yCenter));
	}
}

void Sprite::Load(QDataStream *const dataStream)
{
	QString name;

	*dataStream >> name;

	printf("Name: %s\n", name.toStdString().c_str());

	m_pCurrTex = (Texture*)ResourceView::Get()->GetItem(name);

	RefreshTextureBox();

	for (int i = 0; i < m_textures.size(); ++i)
	{
		if (m_pCurrTex == m_textures[i]->pTex)
		{
			m_ui.textureBox->setCurrentIndex(m_textures[i]->index);
		}
	}

	*dataStream >> m_xCenter >> m_yCenter;
}

void Sprite::Save(QDataStream *const dataStream)
{
	Item::Save(dataStream);

	*dataStream << (m_pCurrTex ? m_pCurrTex->GetName() : QString("")) << m_xCenter << m_yCenter;
}

