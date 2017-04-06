#include "MainWindow.h"

#include <QResizeEvent>
#include <QTime>

#include <ResourceView.h>
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
	m_ui.verticalLayout->addWidget(m_pResView);

	setWindowFlags(windowFlags() | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::Dialog);

	connect(m_ui.actionExit, &QAction::triggered, this, &MainWindow::ActionExit_triggered);
	connect(m_ui.actionNewProject, &QAction::triggered, this, &MainWindow::ActionOpenProject_triggered);
	connect(m_ui.actionOpenProject, &QAction::triggered, this, &MainWindow::ActionOpenProject_triggered);
	connect(m_ui.actionSaveProject, &QAction::triggered, this, &MainWindow::ActionSaveProject_triggered);

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

	printf("Loaded \"%s\" in %d ms \n", m_proInfo->name.toStdString().c_str(), timer.elapsed());

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

void MainWindow::ActionSaveProject_triggered() const
{
	Save();
}

void MainWindow::ActionOpenProject_triggered()
{
	close();
	WelcomeWindow::Get()->show();
}

void MainWindow::ActionExit_triggered()
{
	this->close();
}
