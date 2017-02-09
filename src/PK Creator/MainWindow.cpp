#include "MainWindow.h"

#include <QMessageBox>
#include <QResizeEvent>
#include <QItemSelection>
#include <QModelIndex>

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
	m_pResView->setMinimumSize(QSize(200, 600));
	m_pResView->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
	ui.verticalLayout->addWidget(m_pResView);

	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(ActionExit_triggered()));
	connect(ui.actionSaveProject, SIGNAL(triggered()), this, SLOT(ActionSaveProject_triggered()));

	s_pInst = this;
}

MainWindow::~MainWindow()
{
	if (m_pResView)
	{
		delete m_pResView;
		m_pResView = nullptr;
	}
}

void MainWindow::Load(Project * project)
{
}

void MainWindow::Save()
{
	if (m_proInfo)
		m_pResView->Save(m_proInfo->path + m_proInfo->name + QString("pkp"));
}

void MainWindow::ActionSaveProject_triggered()
{
	Save()
}

void MainWindow::ActionExit_triggered()
{
	this->close();
}
