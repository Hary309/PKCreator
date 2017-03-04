#include "Sprite.h"

#include <QMessageBox>
#include <QStandardItemModel>

#include <ResourceView.h>
#include <QFileDialog>
#include <QDateTime>

Sprite::Sprite(QWidget *parent, QStandardItem *item, const QString &itemName)
	: Item(parent, item, itemName)
{
	m_ui.setupUi(this);

	m_ui.centerXEdit->setValidator(new QIntValidator);
	m_ui.centerYEdit->setValidator(new QIntValidator);

	Sprite::SetName(itemName);

	m_type = Item::SPRITE;
	
	m_size = QSize(0, 0);
	m_center = QPoint(0, 0);

	m_ui.nameEdit->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9]{1,24}")));

	connect(m_ui.okButton, &QPushButton::clicked, this, &Sprite::OkButton_clicked);;
	connect(m_ui.loadSpriteButton, &QPushButton::clicked, this, &Sprite::LoadSpriteButton_clicked);
	connect(m_ui.centerButton, &QPushButton::clicked, this, &Sprite::CenterButton_clicked);

	connect(m_ui.centerXEdit, &QLineEdit::editingFinished, this, &Sprite::CenterXEdit_editingFinished);
	connect(m_ui.centerYEdit, &QLineEdit::editingFinished, this, &Sprite::CenterYEdit_editingFinished);
}


Sprite::~Sprite()
{

}

void Sprite::SetName(const QString & name)
{
	m_itemName = name;
	setWindowTitle(name);
	m_ui.nameEdit->setText(name);
	m_pItem->setText(name);
}

void Sprite::LoadSpriteButton_clicked()
{
	QString filePath = QFileDialog::getOpenFileName(this, "Open image", ResourceView::Get()->GetMainDir(), "Images (*.png *.jpg *.jpeg *.bmp)");

	if (filePath.isEmpty())
		return;

	QDateTime dateTime = QDateTime::currentDateTime();
	QString newFileName;
	newFileName.sprintf("%llx", dateTime.toSecsSinceEpoch());

	m_texPath = QString("resources/") + newFileName + QString(".") + filePath.split(".").last();

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

	m_size = size;
}

void Sprite::CenterButton_clicked()
{
	m_center = QPoint(m_size.width() / 2, m_size.height() / 2);

	m_ui.centerXEdit->setText(QString::number(m_center.x()));
	m_ui.centerYEdit->setText(QString::number(m_center.y()));
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

void Sprite::CenterXEdit_editingFinished()
{
	 m_center.setX(m_ui.centerXEdit->text().toInt());
}

void Sprite::CenterYEdit_editingFinished()
{
	m_center.setY(m_ui.centerYEdit->text().toInt());
}

void Sprite::Load(QDataStream *const dataStream)
{
	*dataStream >> m_size >> m_texPath >> m_center;

	m_ui.imagePathLabel->setText(m_texPath);

	m_ui.widthValueLabel->setText(QString::number(m_size.width()) + "px");
	m_ui.heightValueLabel->setText(QString::number(m_size.height()) + "px");

	m_ui.centerXEdit->setText(QString::number(m_center.x()));
	m_ui.centerYEdit->setText(QString::number(m_center.y()));

	QImage myImage;
	myImage.load(ResourceView::Get()->GetMainDir() + m_texPath);
	m_ui.imageLabel->setPixmap(QPixmap::fromImage(myImage));
}

void Sprite::Save(QDataStream *const dataStream)
{
	Item::Save(dataStream);

	*dataStream << m_size << m_texPath << m_center;
}
