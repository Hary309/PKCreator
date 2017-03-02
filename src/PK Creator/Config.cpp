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

	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	connect(ui.okButton, &QPushButton::clicked, this, [this] { this->hide(); });
}

Config::~Config()
{

}

void Config::Load(QDataStream *const dataStream) const
{
	QString title, width, height;

	*dataStream >> title >> width >> height;

	printf("\"%s\" %dx%d\n", title.toStdString().c_str(), width.toInt(), height.toInt());

	ui.titleEdit->setText(title);
	ui.widthEdit->setText(width);
	ui.heightEdit->setText(height);
}

void Config::Save(QDataStream *const dataStream) const
{
	*dataStream << ui.titleEdit->text() << ui.widthEdit->text() << ui.heightEdit->text();
}

QSize Config::GetWndSize() const
{
	QSize size;
	size.setWidth(ui.widthEdit->text().toInt());
	size.setHeight(ui.heightEdit->text().toInt());

	return size;
}

void Config::OkButton_clicked()
{
	hide();
}
