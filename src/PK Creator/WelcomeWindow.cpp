#include "WelcomeWindow.h"

#include <QFileDialog>
#include <QDateTime>
#include <QMessageBox>
#include <QProcess>

#include <MainWindow.h>

#include <Windows.h>
#include <shellapi.h>

WelcomeWindow *WelcomeWindow::s_pInst;

WelcomeWindow::WelcomeWindow(QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);

	m_pMainWnd = nullptr;

	LoadList();

	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	connect(m_ui.createButton, &QPushButton::clicked, this, &WelcomeWindow::CreateButton_clicked);
	connect(m_ui.openButton, &QPushButton::clicked, this, &WelcomeWindow::OpenButton_clicked);
	connect(m_ui.deleteButton, &QPushButton::clicked, this, &WelcomeWindow::DeleteButton_clicked);
	connect(m_ui.openFolderButton, &QPushButton::clicked, this, &WelcomeWindow::OpenFolderButton_clicked);
	connect(m_ui.exitButton, &QPushButton::clicked, this, [this] { this->close(); } );

	connect(m_ui.projectView, &QTreeWidget::doubleClicked, this, &WelcomeWindow::OpenButton_clicked);

	s_pInst = this;
}

WelcomeWindow::~WelcomeWindow()
{
	SaveList();

	qDeleteAll(m_projectList);
	m_projectList.clear();

	if (m_pMainWnd)
	{
		delete m_pMainWnd;
		m_pMainWnd = nullptr;
	}
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
		pro->item = treeItem;
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



	QFile projectFile(filePath);

	bool isFileValid = false;

	if (projectFile.exists())
	{
		// check version
		projectFile.open(QFile::ReadOnly);

		QDataStream stream(&projectFile);

		QString version, name;

		stream >> version >> name;

		if (version == QString("PKP1") && !name.isEmpty())
		{
			isFileValid = true;
		}

		projectFile.close();
	}

	if (!isFileValid)
	{
		projectFile.open(QFile::WriteOnly);

		QDataStream stream(&projectFile);

		stream << QString("PKP1") << projectName << QString("Game") << QString("800") << QString("600");

		projectFile.close();
	}


	QDateTime currDateTime = QDateTime::currentDateTime();

	QTreeWidgetItem *treeItem = new QTreeWidgetItem();
	treeItem->setText(0, projectName);
	treeItem->setText(1, currDateTime.toString("h:mm:ss dd.MM.yy"));
	treeItem->setText(2, folderPath);

	Project *pro = new Project;
	pro->name = projectName;
	pro->timestamp = currDateTime.toSecsSinceEpoch();
	pro->path = folderPath;
	pro->item = treeItem;

	m_projectList.push_back(pro);

	m_ui.projectView->insertTopLevelItem(m_ui.projectView->topLevelItemCount(), treeItem);

	QDir currDir(folderPath);
	currDir.mkdir("resources");

	SaveList();
}

void WelcomeWindow::OpenButton_clicked()
{
	if (m_ui.projectView->selectedItems().isEmpty())
	{
		QMessageBox::information(this, "PK Creator", "Please select project!");
		return;
	}

	QTreeWidgetItem *treeItem = m_ui.projectView->selectedItems().first();

	for (int i = 0; i < m_projectList.size(); ++i)
	{
		if (m_projectList[i])
		{
			if (m_projectList[i]->item == treeItem)
			{
				if (m_pMainWnd)
				{
					delete m_pMainWnd;
					m_pMainWnd = nullptr;
				}

				m_pMainWnd = new MainWindow();

				if (!m_pMainWnd->Load(m_projectList[i]))
				{
					QMessageBox::critical(this, "Error", "Cannot open project!");
					
					delete m_pMainWnd;
					m_pMainWnd = nullptr;

					return;
				}

				m_pMainWnd->show();
				this->hide();
			}
		}
	}
}

void WelcomeWindow::DeleteButton_clicked()
{
	if (m_ui.projectView->selectedItems().isEmpty())
	{
		QMessageBox::information(this, "PK Creator", "Please select project!");
		return;
	}

	if (QMessageBox::question(this, "Pk Creator", "Are you sure you want to remove project with all files?") == QMessageBox::No)
		return;

	QTreeWidgetItem *treeItem = m_ui.projectView->selectedItems().first();

	for (int i = 0; i < m_projectList.size(); ++i)
	{
		if (m_projectList[i])
		{
			if (m_projectList[i]->item == treeItem)
			{
				QDir dir(m_projectList[i]->path + "resources");
				dir.removeRecursively();

				QFile file(m_projectList[i]->path + m_projectList[i]->name + ".pkp");
				file.remove();

				delete m_projectList[i];
				m_projectList[i] = nullptr;

				m_projectList.removeAt(i);
			
				delete treeItem;
				treeItem = nullptr;
			}
		}
	}

	SaveList();
}

void WelcomeWindow::OpenFolderButton_clicked()
{
	if (m_ui.projectView->selectedItems().isEmpty())
	{
		QMessageBox::information(this, "PK Creator", "Please select project!");
		return;
	}

	QTreeWidgetItem *treeItem = m_ui.projectView->selectedItems().first();

	for (int i = 0; i < m_projectList.size(); ++i)
	{
		if (m_projectList[i])
		{
			if (m_projectList[i]->item == treeItem)
			{
				ShellExecuteA(nullptr, "open", m_projectList[i]->path.toStdString().c_str(), nullptr, nullptr, SW_SHOWDEFAULT);
			}
		}
	}
}
