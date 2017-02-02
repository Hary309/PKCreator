#include "EventItem.h"

#include <QCloseEvent>
#include <QMessageBox>

#include <CodeEditor.h>

EventItem::EventItem(QWidget *parent, Type type, QStandardItem *item)
	: CodeEditorWindow(parent), m_type(type), m_item(item)
{
	connect(m_ui.actionOk, &QAction::triggered, this, &EventItem::ActionOk_triggered);
	connect(m_ui.actionSave, &QAction::triggered, this, &EventItem::ActionSave_triggered);
}

EventItem::~EventItem()
{
}

void EventItem::ActionSave_triggered()
{
	if (m_codeChanged)
	{
		QString title = windowTitle().split('*').first();

		setWindowTitle(title);
	}

	m_code = m_pCodeEditor->toPlainText();
	m_codeChanged = false;
}

void EventItem::ActionOk_triggered()
{
	m_code = m_pCodeEditor->toPlainText();

	this->hide();
}

void EventItem::closeEvent(QCloseEvent *closeEvent)
{
	QString newCode = m_pCodeEditor->toPlainText();

	if (newCode != m_code)
	{
		QMessageBox::StandardButtons result = QMessageBox::question(this, "PK Creator", "Do you want to save the changes?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

		if (result == QMessageBox::Yes)
		{
			m_code = newCode;

			closeEvent->accept();
		}
		else if (result == QMessageBox::No)
		{
			m_pCodeEditor->setPlainText(m_code);

			closeEvent->accept();
		}
		else if (result == QMessageBox::Cancel)
		{
			closeEvent->ignore();
		}
	}
}

void EventItem::keyPressEvent(QKeyEvent *e)
{
	if ((e->key() == Qt::Key_S) && QApplication::keyboardModifiers() && Qt::ControlModifier)
	{
		ActionSave_triggered();
	}
}