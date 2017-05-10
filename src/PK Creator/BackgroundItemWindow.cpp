/*
*********************************************************************
* File          : BackgroundItemWindow.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "BackgroundItemWindow.h"

#include <QMessageBox>

#include <ResourceView.h>
#include <BackgroundItem.h>

#include <QFileDialog>


BackgroundItemWindow::BackgroundItemWindow(QWidget *parent)
	: ItemWindow(parent)
{
	m_ui.setupUi(this);

	m_ui.nameEdit->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9]{1,24}")));

	m_pItemParent = nullptr;

	connect(m_ui.okButton, &QPushButton::clicked, this, &BackgroundItemWindow::OkButton_clicked);;
	connect(m_ui.loadBgButton, &QPushButton::clicked, this, &BackgroundItemWindow::LoadBgButton_clicked);
}

BackgroundItemWindow::~BackgroundItemWindow()
{
}

bool BackgroundItemWindow::FillData(Item *item)
{
	m_pItemParent = static_cast<BackgroundItem*>(item);

	m_ui.nameEdit->setText(m_pItemParent->GetName());

	m_ui.imagePathLabel->setText(m_pItemParent->m_texPath);

	m_ui.widthValueLabel->setText(QString::number(m_pItemParent->m_size.width()) + "px");
	m_ui.heightValueLabel->setText(QString::number(m_pItemParent->m_size.height()) + "px");

	QImage myImage;
	myImage.load(ResourceView::Get()->GetMainDir() + m_pItemParent->m_texPath);
	m_ui.imageLabel->setPixmap(QPixmap::fromImage(myImage));
	m_ui.imageLabel->setFixedSize(myImage.size());

	return true;
}


void BackgroundItemWindow::closeEvent(QCloseEvent *event)
{
	m_pItemParent->Close();
}

void BackgroundItemWindow::LoadBgButton_clicked()
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
	m_ui.imageLabel->setFixedSize(myImage.size());

	QSize size = myImage.size();
	m_ui.widthValueLabel->setText(QString::number(size.width()) + "px");
	m_ui.heightValueLabel->setText(QString::number(size.height()) + "px");

	m_pItemParent->m_size = size;
}

void BackgroundItemWindow::OkButton_clicked()
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
