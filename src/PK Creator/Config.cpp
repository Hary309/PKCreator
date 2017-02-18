#include "Config.h"

Config::Config(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	if (parent)
	{
		ui.titleEdit->setText("Game");
	}

	ui.widthEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]{2,4}")));
	ui.heightEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]{2,4}")));

	ui.widthEdit->setText("800");
	ui.heightEdit->setText("600");

	connect(ui.okButton, &QPushButton::clicked, this, [this] { this->hide(); });
}

Config::~Config()
{

}

void Config::Load(QDataStream *const dataStream)
{
	QString title, width, height;

	*dataStream >> title >> width >> height;

	printf("\"%s\" %dx%d\n", title.toStdString().c_str(), width.toInt(), height.toInt());

	ui.titleEdit->setText(title);
	ui.widthEdit->setText(width);
	ui.heightEdit->setText(height);
}

void Config::Save(QDataStream *const dataStream)
{
	*dataStream << ui.titleEdit->text() << ui.widthEdit->text() << ui.heightEdit->text();
}

void Config::OkButton_clicked()
{
	hide();
}
