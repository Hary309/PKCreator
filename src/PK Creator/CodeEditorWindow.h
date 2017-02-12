#pragma once

#include <QMainWindow>

#include <ui_CodeWindow.h>

class QTimer;

class CodeEditor;
class Highlighter;

class CodeEditorWindow : public QMainWindow
{
protected:
	Ui::CodeWindow		m_ui;

	CodeEditor			*m_pCodeEditor;
	Highlighter			*m_pHighlighter;

	QTimer				*m_pTimer;

	bool				m_codeChanged;

public:
	CodeEditorWindow(QWidget *parent);
	~CodeEditorWindow();

private slots:
	virtual void ActionOk_triggered() = 0;
	virtual void ActionSave_triggered() = 0;

	void ActionSaveAs_triggered();
	void ActionOpen_triggered();
	void CodeEditor_textChanged();

	void Timer_timeout();
};

