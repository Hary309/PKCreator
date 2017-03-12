#include "SpriteItemWindow.h"

#include <QMessageBox>

#include <ResourceView.h>
#include <QFileDialog>

#include <SpriteItem.h>
#include <Windows.h>

SpriteItemWindow::SpriteItemWindow(QWidget *parent)
	: ItemWindow(parent)
{
	m_ui.setupUi(this);

	m_ui.centerXEdit->setValidator(new QIntValidator);
	m_ui.centerYEdit->setValidator(new QIntValidator);
	m_ui.nameEdit->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9]{1,24}")));

	m_pItemParent = nullptr;

	connect(m_ui.okButton, &QPushButton::clicked, this, &SpriteItemWindow::OkButton_clicked);;
	connect(m_ui.loadSpriteButton, &QPushButton::clicked, this, &SpriteItemWindow::LoadSpriteButton_clicked);
	connect(m_ui.centerButton, &QPushButton::clicked, this, &SpriteItemWindow::CenterButton_clicked);

	connect(m_ui.centerXEdit, &QLineEdit::editingFinished, this, &SpriteItemWindow::CenterXEdit_editingFinished);
	connect(m_ui.centerYEdit, &QLineEdit::editingFinished, this, &SpriteItemWindow::CenterYEdit_editingFinished);
}


SpriteItemWindow::~SpriteItemWindow()
{
}

bool SpriteItemWindow::FillData(Item *item)
{
	m_pItemParent = static_cast<SpriteItem*>(item);

	if (!m_pItemParent)
		return false;

	if (m_pItemParent->m_type != Item::SPRITE)
		return false;

	m_ui.nameEdit->setText(m_pItemParent->GetName());

	m_ui.imagePathLabel->setText(m_pItemParent->m_texPath);

	m_ui.widthValueLabel->setText(QString::number(m_pItemParent->m_size.width()) + "px");
	m_ui.heightValueLabel->setText(QString::number(m_pItemParent->m_size.height()) + "px");

	m_ui.centerXEdit->setText(QString::number(m_pItemParent->m_center.x()));
	m_ui.centerYEdit->setText(QString::number(m_pItemParent->m_center.y()));

	QImage myImage;
	myImage.load(ResourceView::Get()->GetMainDir() + m_pItemParent->m_texPath);
	m_ui.imageLabel->setPixmap(QPixmap::fromImage(myImage));

	return true;
}

void SpriteItemWindow::closeEvent(QCloseEvent* event)
{
	m_pItemParent->Close();
}

void SpriteItemWindow::LoadSpriteButton_clicked()
{
	QString filePath = QFileDialog::getOpenFileName(this, "Open image", ResourceView::Get()->GetMainDir(), "Images (*.png *.jpg *.jpeg *.bmp)");

	if (filePath.isEmpty())
		return;

	QDateTime dateTime = QDateTime::currentDateTime();
	QString newFileName;
	newFileName.sprintf("%llx", dateTime.toSecsSinceEpoch());

	m_pItemParent->m_texPath = QString("resources/") + newFileName + QString(".") + filePath.split(".").last();

	QFile file(ResourceView::Get()->GetMainDir() + m_pItemParent->m_texPath);
	file.remove();

	QFile::copy(filePath, ResourceView::Get()->GetMainDir() + m_pItemParent->m_texPath);

	m_ui.imagePathLabel->setText(m_pItemParent->m_texPath);

	QImage myImage;
	myImage.load(filePath);
	m_ui.imageLabel->setPixmap(QPixmap::fromImage(myImage));

	QSize size = myImage.size();
	m_ui.widthValueLabel->setText(QString::number(size.width()) + "px");
	m_ui.heightValueLabel->setText(QString::number(size.height()) + "px");

	m_pItemParent->m_size = size;
}

void SpriteItemWindow::CenterButton_clicked()
{
	m_pItemParent->m_center = QPoint(m_pItemParent->m_size.width() / 2, m_pItemParent->m_size.height() / 2);

	m_ui.centerXEdit->setText(QString::number(m_pItemParent->m_center.x()));
	m_ui.centerYEdit->setText(QString::number(m_pItemParent->m_center.y()));
}

void SpriteItemWindow::OkButton_clicked()
{
	QString name = m_ui.nameEdit->text();

	if (ResourceView::Get()->IsNameExists(name) && name != m_pItemParent->m_itemName)
	{
		QMessageBox::information(this, "PK Creator", "This name already exists!");
		return;
	}

	m_pItemParent->SetName(name);
	m_pItemParent->Close();
}

void SpriteItemWindow::CenterXEdit_editingFinished()
{
	m_pItemParent->m_center.setX(m_ui.centerXEdit->text().toInt());
}

void SpriteItemWindow::CenterYEdit_editingFinished()
{

	m_pItemParent->m_center.setY(m_ui.centerYEdit->text().toInt());
}
