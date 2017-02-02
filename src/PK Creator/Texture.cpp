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

	connect(m_ui.okButton, SIGNAL(clicked()), this, SLOT(OkButton_clicked()));
	connect(m_ui.loadButton, SIGNAL(clicked()), this, SLOT(LoadButton_clicked()));
}

Texture::~Texture()
{
}

void Texture::SetName(const QString &name)
{
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
		QMessageBox::information(this, tr("PK Creator"), tr("This name already exists!"));
		return;
	}

	SetName(name);
	hide();
}

void Texture::CancelButton_clicked()
{
	m_ui.nameEdit->setText(m_itemName);
	hide();
}

void Texture::LoadButton_clicked()
{
	QString filePath = QFileDialog::getOpenFileName(this, "Open image", QDir::currentPath(), "All (*.*)");

	if (filePath.isEmpty())
		return;

	QFileInfo fileInfo(filePath);
	m_ui.imagePathLabel->setText(fileInfo.fileName());

	QImage myImage;
	myImage.load(filePath);
	m_ui.imageLabel->setPixmap(QPixmap::fromImage(myImage));

	QSize size = myImage.size();	
	m_ui.widthValueLabel->setText(QString::number(size.width()) + "px");
	m_ui.heightValueLabel->setText(QString::number(size.height()) + "px");
}
