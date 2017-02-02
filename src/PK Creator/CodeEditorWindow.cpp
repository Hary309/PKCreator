#include "CodeEditorWindow.h"

#include <QFileDialog>

#include <CodeEditor.h>
#include <Highlighter.h>

CodeEditorWindow::CodeEditorWindow(QWidget *parent)
	: QMainWindow(parent)
{
	m_ui.setupUi(this);

	m_pCodeEditor = new CodeEditor(this);

	m_ui.gridLayout->addWidget(m_pCodeEditor);

	m_pHighlighter = new Highlighter(m_pCodeEditor->document());

	connect(m_ui.actionCut, &QAction::triggered, m_pCodeEditor, &QPlainTextEdit::cut);
	connect(m_ui.actionCopy, &QAction::triggered, m_pCodeEditor, &QPlainTextEdit::copy);
	connect(m_ui.actionPaste, &QAction::triggered, m_pCodeEditor, &QPlainTextEdit::paste);

	connect(m_ui.actionOpen, &QAction::triggered, this, &CodeEditorWindow::ActionOpen_triggered);
}


CodeEditorWindow::~CodeEditorWindow()
{
	if (m_pCodeEditor)
	{
		delete m_pCodeEditor;
		m_pCodeEditor = nullptr;
	}
}

void CodeEditorWindow::ActionOpen_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open lua script"), QDir::currentPath(), tr("Lua file (*.lua)"));

	QFile file(fileName);
	file.open(QFile::ReadOnly);

	m_pCodeEditor->setPlainText(file.readAll());
}
