#include "EventItemWindow.h"

#include <QMessageBox>
#include <QStandardItem>

#include <EventItem.h>

#include <CodeEditor.h>

EventItemWindow::EventItemWindow(QWidget *parent)
	: CodeEditorWindow(parent)
{
	m_pItemParent = nullptr;

	connect(m_ui.actionOk, &QAction::triggered, this, &EventItemWindow::ActionOk_triggered);
	connect(m_ui.actionSave, &QAction::triggered, this, &EventItemWindow::ActionSave_triggered);
}

EventItemWindow::~EventItemWindow()
{
}

void EventItemWindow::FillData(EventItem *item)
{
	m_pItemParent = item;

	m_pCodeEditor->setPlainText(m_pItemParent->m_code);

	setWindowTitle(QString::asprintf("%s - Event", m_pItemParent->GetItem()->text().toStdString().c_str()));
}

void EventItemWindow::closeEvent(QCloseEvent *e)
{
	QString newCode = m_pCodeEditor->toPlainText();

	if (newCode != m_pItemParent->m_code)
	{
		QMessageBox::StandardButtons result = QMessageBox::question(this, "PK Creator", "Do you want to save the changes?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

		if (result == QMessageBox::Yes)
		{
			m_pItemParent->m_code = newCode;

			e->accept();
		}
		else if (result == QMessageBox::No)
		{
			m_pCodeEditor->setPlainText(m_pItemParent->m_code);

			e->accept();
		}
		else if (result == QMessageBox::Cancel)
		{
			e->ignore();
		}
	}

	m_pItemParent->Close();
}

void EventItemWindow::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_S && QApplication::keyboardModifiers() == Qt::ControlModifier)
	{
		ActionSave_triggered();
	}
}

void EventItemWindow::ActionSave_triggered()
{
	if (m_codeChanged)
	{
		QString title = windowTitle().split('*').first();

		setWindowTitle(title);
	}

	m_pItemParent->m_code = m_pCodeEditor->toPlainText();
	m_codeChanged = false;
}

void EventItemWindow::ActionOk_triggered()
{
	m_pItemParent->m_code = m_pCodeEditor->toPlainText();

	m_pItemParent->Close();
}