#pragma once

#include <CodeEditor.h>

class LineNumberArea : public QWidget
{
public:
	LineNumberArea(CodeEditor *editor) : QWidget(editor) {
		codeEditor = editor;
	}

	QSize sizeHint() const override {
		return QSize(codeEditor->LineNumberAreaWidth(), 0);
	}

protected:
	void paintEvent(QPaintEvent *event) override {
		codeEditor->LineNumberAreaPaintEvent(event);
	}

private:
	CodeEditor *codeEditor;
};