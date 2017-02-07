#include "WelcomeWindow.h"

#include <QFileDialog>
#include <QDateTime>
#include <QMessageBox>

WelcomeWindow::WelcomeWindow(QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);

	LoadList();

	connect(m_ui.createButton, &QPushButton::clicked, this, &WelcomeWindow::CreateButton_clicked);
	connect(m_ui.exitButton, &QPushButton::clicked, this, [this] { this->close(); } );
}

WelcomeWindow::~WelcomeWindow()
{
	SaveList();

	qDeleteAll(m_projectList);
	m_projectList.clear();
}

void WelcomeWindow::SaveList()
{
	// pkb - pk binary
	QFile file("list.pkb");

	file.open(QFile::WriteOnly);

	QDataStream stream(&file);

	stream << m_projectList.size();

	for (int i = 0; i < m_projectList.size(); ++i)
	{
		stream << m_projectList[i]->name << m_projectList[i]->timestamp << m_projectList[i]->path;
	}

	file.close();
}

void WelcomeWindow::LoadList()
{
	QFile file("list.pkb");

	if (!file.exists())
		return;

	if (file.error() != QFile::NoError)
	{
		QMessageBox::critical(this, "File read error!", file.errorString());
		return;
	}

	file.open(QFile::ReadOnly);

	QDataStream stream(&file); 

	int size = 0;

	stream >> size;

	for (int i = 0; i < size; ++i)
	{
		QString name, path;
		qint64 timestamp;

		stream >> name >> timestamp >> path;
	
		QDateTime time;
		time.setSecsSinceEpoch(timestamp);

		QTreeWidgetItem *treeItem = new QTreeWidgetItem();
		treeItem->setText(0, name);
		treeItem->setText(1, time.toString("h:mm:ss dd.MM.yy"));
		treeItem->setText(2, path);

		Project *pro = new Project;
		pro->name = name;
		pro->timestamp = timestamp;
		pro->path = path;
		m_projectList.push_back(pro);

		m_ui.projectView->insertTopLevelItem(m_ui.projectView->topLevelItemCount(), treeItem);

	}

	file.close();
}

void WelcomeWindow::CreateButton_clicked()
{
	QString filePath = QFileDialog::getSaveFileName(this, "Choose location and name of project", QDir::currentPath(), "PK Project (*.pkp)");

	if (filePath.isEmpty())
		return;

	QString fileName = filePath.split("/").last();
	QString projectName = fileName.split(".").first();
	QString folderPath = filePath.left(filePath.length() - fileName.length());

	QDateTime currDateTime = QDateTime::currentDateTime();

	QTreeWidgetItem *treeItem = new QTreeWidgetItem();
	treeItem->setText(0, projectName);
	treeItem->setText(1, currDateTime.toString("h:mm:ss dd.MM.yy"));
	treeItem->setText(2, folderPath);

	Project *pro = new Project;
	pro->name = projectName;
	pro->timestamp = currDateTime.toSecsSinceEpoch();
	pro->path = folderPath;
	m_projectList.push_back(pro);

	m_ui.projectView->insertTopLevelItem(m_ui.projectView->topLevelItemCount(), treeItem);
}
