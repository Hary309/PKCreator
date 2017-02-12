#include "CodeEditorWindow.h"

#include <QFileDialog>
#include <QTimer>

#include <ResourceView.h>
#include <CodeEditor.h>
#include <Highlighter.h>

#include <LuaDebugger.h>

CodeEditorWindow::CodeEditorWindow(QWidget *parent)
	: QMainWindow(parent)
{
	m_ui.setupUi(this);

	m_pCodeEditor = new CodeEditor(this);

	m_ui.gridLayout->addWidget(m_pCodeEditor);

	m_pHighlighter = new Highlighter(m_pCodeEditor->document());

	m_codeChanged = false;

	m_pTimer = new QTimer(this);
	m_pTimer->setSingleShot(true);

	connect(m_pTimer, &QTimer::timeout, this, &CodeEditorWindow::Timer_timeout);

	connect(m_ui.actionCut, &QAction::triggered, m_pCodeEditor, &QPlainTextEdit::cut);
	connect(m_ui.actionCopy, &QAction::triggered, m_pCodeEditor, &QPlainTextEdit::copy);
	connect(m_ui.actionPaste, &QAction::triggered, m_pCodeEditor, &QPlainTextEdit::paste);

	connect(m_ui.actionSaveAs, &QAction::triggered, this, &CodeEditorWindow::ActionSaveAs_triggered);
	connect(m_ui.actionOpen, &QAction::triggered, this, &CodeEditorWindow::ActionOpen_triggered);
	connect(m_pCodeEditor, &QPlainTextEdit::textChanged, this, &CodeEditorWindow::CodeEditor_textChanged);
}


CodeEditorWindow::~CodeEditorWindow()
{
	if (m_pCodeEditor)
	{
		delete m_pCodeEditor;
		m_pCodeEditor = nullptr;
	}
}

void CodeEditorWindow::ActionSaveAs_triggered()
{
	 QString filePath = QFileDialog::getSaveFileName(this, "Save lua script", QDir::currentPath(), "Lua file (*.lua)");

	 if (filePath.isEmpty())
		 return;

	QFile file(filePath);
	file.open(QFile::WriteOnly);	
	file.write(m_pCodeEditor->toPlainText().toStdString().c_str());
	file.close();
}

void CodeEditorWindow::ActionOpen_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open lua script", QDir::currentPath(), "Lua file (*.lua)");

	if (fileName.isEmpty())
		return;

	QFile file(fileName);
	file.open(QFile::ReadOnly);
	m_pCodeEditor->setPlainText(file.readAll());
	file.close();
}

void CodeEditorWindow::CodeEditor_textChanged()
{
	if (!m_codeChanged)
	{
		setWindowTitle(windowTitle() + "*");
	}

	m_codeChanged = true;

	m_pTimer->start(1000);
}

void CodeEditorWindow::Timer_timeout()
{
	QString error = ResourceView::Get()->GetLuaDebugger()->TestScript(m_pCodeEditor->toPlainText(), QString("@script"));
	
	statusBar()->showMessage(error);
}
