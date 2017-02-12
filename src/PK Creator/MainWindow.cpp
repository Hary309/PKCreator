#include "MainWindow.h"

#include <QMessageBox>
#include <QResizeEvent>
#include <QItemSelection>
#include <QModelIndex>
#include <QTime>

#include <ResourceView.h>
#include <Object.h>
#include <WelcomeWindow.h>


MainWindow *MainWindow::s_pInst;

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	m_pResView = new ResourceView(ui.centralWidget);
	m_pResView->setObjectName(QStringLiteral("resourceView"));
	m_pResView->setEnabled(true);
	m_pResView->setMouseTracking(true);
	m_pResView->setMaximumSize(QSize(200, 16777215));
	m_pResView->setMinimumSize(QSize(200, 400));
	m_pResView->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
	ui.verticalLayout->addWidget(m_pResView);

	setWindowFlags(windowFlags() | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::Dialog);

	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(ActionExit_triggered()));
	connect(ui.actionSaveProject, SIGNAL(triggered()), this, SLOT(ActionSaveProject_triggered()));
	connect(ui.actionOpenProject, SIGNAL(triggered()), this, SLOT(ActionOpenProject_triggered()));

	s_pInst = this;
}

MainWindow::~MainWindow()
{
	if (m_pResView)
	{
		delete m_pResView;
		m_pResView = nullptr;
	}

	s_pInst = nullptr;
}

void MainWindow::Load(Project * project)
{
	m_proInfo = project;

	QTime timer;
	timer.start();

	if (m_proInfo)
	{
		printf("Opening...\n");

		QFile file(m_proInfo->path + m_proInfo->name + QString(".pkp"));
		
		if (!file.exists())
			return;

		file.open(QFile::ReadOnly);

		if (file.error() != QFile::NoError)
			return;

		printf("Opened!\n");

		QDataStream stream(&file);

		QString version, name;
		stream >> version >> name;
		printf("Version: \"%s\"\n", version.toStdString().c_str());

		if (version != "PKP1")
			return;

		printf("Version checked!\n");

		m_pResView->Load(&stream, project->path);

		file.close();
	}


	printf("Loaded \"%s\" in %d ms \n", m_proInfo->name.toStdString().c_str(), timer.elapsed());
}

void MainWindow::Save()
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

void MainWindow::ActionSaveProject_triggered()
{
	Save();
}

void MainWindow::ActionOpenProject_triggered()
{
	WelcomeWindow::Get()->show();
	this->hide();
}

void MainWindow::ActionExit_triggered()
{
	this->close();
}
