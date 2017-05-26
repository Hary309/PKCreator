/*
*********************************************************************
* File          : MainWindow.cpp
* Project		: PK Creator
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "MainWindow.h"

#include <QResizeEvent>
#include <QTime>
#include <QMessageBox>

#include <Config.h>
#include <ResourceView.h>
#include <HTML5Generator.h>
#include <WelcomeWindow.h>

MainWindow *MainWindow::s_pInst;

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	m_ui.setupUi(this);

	m_pResView = new ResourceView(m_ui.centralWidget);
	m_pResView->setObjectName(QStringLiteral("resourceView"));
	m_pResView->setEnabled(true);
	m_pResView->setMouseTracking(true);
	m_pResView->setMaximumSize(QSize(200, 16777215));
	m_pResView->setMinimumSize(QSize(200, 400));
	m_pResView->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
	m_ui.verticalLayout_2->addWidget(m_pResView);

	setWindowFlags(windowFlags() | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::Dialog);

	connect(m_ui.actionNewProject, &QAction::triggered, this, &MainWindow::ActionOpenProject_triggered);
	connect(m_ui.actionOpenProject, &QAction::triggered, this, &MainWindow::ActionOpenProject_triggered);
	connect(m_ui.actionSaveProject, &QAction::triggered, this, [this] { Save(); });
	connect(m_ui.actionExit, &QAction::triggered, this, [this] { close(); });
	connect(m_ui.actionOpenContainingFolder, &QAction::triggered, this, &MainWindow::ActionOpenContainingFolder_triggered);
	connect(m_ui.actionGenerateCode, &QAction::triggered, this, [this] { GenerateCode(true); });
	connect(m_ui.actionRun, &QAction::triggered, this, &MainWindow::ActionRun_triggered);
	connect(m_ui.actionConfig, &QAction::triggered, this, &MainWindow::ActionConfig_triggered);

	s_pInst = this;
}

MainWindow::~MainWindow()
{
	s_pInst = nullptr;
}

bool MainWindow::Load(Project *project)
{
	m_proInfo = project;

	QTime timer;
	timer.start();

	if (m_proInfo)
	{
		printf("Opening...\n");

		QFile file(m_proInfo->path + m_proInfo->name + QString(".pkp"));
		
		if (!file.exists())
			return false;

		file.open(QFile::ReadOnly);

		if (file.error() != QFile::NoError)
			return false;

		printf("Opened!\n");

		QDataStream stream(&file);

		QString version, name;
		stream >> version >> name;
		printf("Version: \"%s\"\n", version.toStdString().c_str());

		if (version != "PKP1")
			return false;

		printf("Version checked!\n");

		if (!m_pResView->Load(&stream, project->path))
		{
			file.close();
			return false;
		}

		file.close();
	}

	if (m_proInfo)
		printf("Loaded \"%s\" in %d ms \n", m_proInfo->name.toStdString().c_str(), timer.elapsed());
	else
		printf("Cannot load!\n");

	return true;
}

void MainWindow::Save() const
{
	QTime timer;
	timer.start();

	if (m_proInfo)
	{
		QFile file(m_proInfo->path + m_proInfo->name + QString(".pkp"));
		file.open(QFile::WriteOnly);

		QDataStream stream(&file);

		// version id
		stream << QString("PKP1") << m_proInfo->name;

		m_pResView->Save(&stream);

		file.close();
	}

	printf("Saved in: %d ms\n", timer.elapsed());
}

void MainWindow::GenerateCode(bool showInformation)
{
	QTime timer;
	timer.start();

	printf("Generating code...\n");

	if (m_pResView && m_proInfo)
	{
		printf("Creating folder...\n");
		HTML5Generator codeGenerator(m_proInfo->path + "\\Generated");
		m_pResView->GenerateCode(&codeGenerator);

		printf("Saving...\n");
		codeGenerator.Save();
	}
	else
	{
		printf("Fail!\n");
		return;
	}

	printf("Generated in: %d ms\n", timer.elapsed());

	if (showInformation)
		QMessageBox::information(this, "Code generator", "Code successfully generated!");
}

void MainWindow::ActionOpenProject_triggered()
{
	close();
	WelcomeWindow::Get()->show();
}

void MainWindow::ActionOpenContainingFolder_triggered()
{
	if (m_proInfo)
		ShellExecuteA(nullptr, "open", m_proInfo->path.toStdString().c_str(), nullptr, nullptr, SW_SHOWDEFAULT);
}

void MainWindow::ActionConfig_triggered()
{
	if (m_pResView)
	{
		if (m_pResView->GetConfig())
			m_pResView->GetConfig()->show();
	}
}

void MainWindow::ActionRun_triggered()
{
	if (m_proInfo)
	{
		GenerateCode(false);

		ShellExecuteA(0, 0, QString(m_proInfo->path + "\\Generated\\HTML5\\index.html").toStdString().c_str(), 0, 0, SW_SHOW);
	}
}
