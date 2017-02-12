#include "Texture.h"

#include <QMessageBox>
#include <QFileDialog>

#include <ResourceView.h>

Texture::Texture(QWidget *parent, QStandardItem *item, const QString &itemName)
	: Item(parent, item, itemName)
{
	m_ui.setupUi(this);

	SetName(itemName);

	m_type = Item::TEXTURE;

	m_width = -1;
	m_height = -1;

	m_texPath = QString();

	m_ui.nameEdit->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9]{1,24}")));

	connect(m_ui.okButton, SIGNAL(clicked()), this, SLOT(OkButton_clicked()));
	connect(m_ui.loadButton, SIGNAL(clicked()), this, SLOT(LoadButton_clicked()));
}

Texture::~Texture()
{
}

void Texture::SetName(const QString &name)
{
	if (!m_texPath.isEmpty())
	{
		QString resPath = ResourceView::Get()->GetMainDir();

		QString newPath = QString("resources/") + name + QString(".") + m_texPath.split(".").last();

		QFile::rename(resPath + m_texPath, resPath + newPath);
	
		m_texPath = newPath;
	}

	m_itemName = name;
	setWindowTitle(name);
	m_ui.nameEdit->setText(name);
	m_pItem->setText(name);
}

void Texture::OkButton_clicked()
{
	QString name = m_ui.nameEdit->text();

	if (ResourceView::Get()->IsNameExists(name) && name != m_itemName)
	{
		QMessageBox::information(this, "PK Creator", "This name already exists!");
		return;
	}

	SetName(name);
	hide();
	accept();
}

void Texture::LoadButton_clicked()
{
	QString filePath = QFileDialog::getOpenFileName(this, "Open image", ResourceView::Get()->GetMainDir(), "Images (*.png *.jpg *.jpeg *.bmp)");

	if (filePath.isEmpty())
		return;
	
	m_texPath = QString("resources/") + m_itemName + QString(".") + filePath.split(".").last();

	QFile file(ResourceView::Get()->GetMainDir() + m_texPath);
	file.remove();

	QFile::copy(filePath, ResourceView::Get()->GetMainDir() + m_texPath);
	
	m_ui.imagePathLabel->setText(m_texPath);

	QImage myImage;
	myImage.load(filePath);
	m_ui.imageLabel->setPixmap(QPixmap::fromImage(myImage));

	QSize size = myImage.size();	
	m_ui.widthValueLabel->setText(QString::number(size.width()) + "px");
	m_ui.heightValueLabel->setText(QString::number(size.height()) + "px");

	m_width = size.width();
	m_height = size.height();
}

void Texture::Load(QDataStream *const dataStream)
{
	*dataStream >> m_texPath >> m_width >> m_height;

	if (m_texPath.isEmpty())
		return;
		
	m_ui.imagePathLabel->setText(m_texPath);

	QImage myImage;
	myImage.load(ResourceView::Get()->GetMainDir() + m_texPath);
	m_ui.imageLabel->setPixmap(QPixmap::fromImage(myImage));

	m_ui.widthValueLabel->setText(QString::number(m_width) + "px");
	m_ui.heightValueLabel->setText(QString::number(m_height) + "px");
}

void Texture::Save(QDataStream *const dataStream)
{
	Item::Save(dataStream);

	*dataStream << m_texPath << m_width << m_height;
}
