#include "CodeEditor.h"

#include <QPainter>
#include <QTextBlock>

#include "LineNumberArea.h"

CodeEditor::CodeEditor(QWidget *parent)
	: QPlainTextEdit(parent)
{
	m_pLineNumberArea = new LineNumberArea(this);

	SetupEditor();

	UpdateLineNumberAreaWidth(0);
	HighlightCurrentLine();

	connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(UpdateLineNumberAreaWidth(int)));
	connect(this, SIGNAL(updateRequest(QRect, int)), this, SLOT(UpdateLineNumberArea(QRect, int)));
	connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(HighlightCurrentLine()));
}


CodeEditor::~CodeEditor()
{
	if (m_pLineNumberArea)
	{
		delete m_pLineNumberArea;
		m_pLineNumberArea = nullptr;
	}
}

int CodeEditor::LineNumberAreaWidth()
{
	int digits = 1;
	int max = qMax<int>(1, blockCount());
	while (max >= 10) 
	{
		max /= 10;
		++digits;
	}

	int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

	return space;
}

void CodeEditor::UpdateLineNumberAreaWidth(int /* newBlockCount */)
{
	setViewportMargins(LineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::UpdateLineNumberArea(const QRect &rect, int dy)
{
	if (dy)
		m_pLineNumberArea->scroll(0, dy);
	else
		m_pLineNumberArea->update(0, rect.y(), m_pLineNumberArea->width(), rect.height());

	if (rect.contains(viewport()->rect()))
		UpdateLineNumberAreaWidth(0);
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{
	QPlainTextEdit::resizeEvent(e);

	QRect cr = contentsRect();
	m_pLineNumberArea->setGeometry(QRect(cr.left(), cr.top(), LineNumberAreaWidth(), cr.height()));
}

void CodeEditor::SetupEditor()
{
	QFont font;

	int id = QFontDatabase::addApplicationFont(":/CodeWindow/res/DejaVuSansMono.ttf");
	QString family = QFontDatabase::applicationFontFamilies(id).first();

	font.setFamily(family);
	font.setFixedPitch(true);
	font.setStyleHint(QFont::Monospace);
	font.setPointSize(10);

	setFont(font);

	// set tab size
	QFontMetrics metrics(font);
	setTabStopWidth(4 * metrics.width(' '));
}

void CodeEditor::HighlightCurrentLine()
{
	QList<QTextEdit::ExtraSelection> extraSelections;

	if (!isReadOnly()) 
	{
		QTextEdit::ExtraSelection selection;

		QColor lineColor;
		lineColor.setRgb(232, 232, 255);

		selection.format.setBackground(lineColor);
		selection.format.setProperty(QTextFormat::FullWidthSelection, true);
		selection.cursor = textCursor();
		selection.cursor.clearSelection();
		extraSelections.append(selection);
	}

	setExtraSelections(extraSelections);
}

void CodeEditor::LineNumberAreaPaintEvent(QPaintEvent *event)
{
	QPainter painter(m_pLineNumberArea);
	QColor numberAreaColor;
	numberAreaColor.setRgb(226, 229, 225);

	painter.fillRect(event->rect(), numberAreaColor);


	QTextBlock block = firstVisibleBlock();
	int blockNumber = block.blockNumber();
	int top = (int)blockBoundingGeometry(block).translated(contentOffset()).top();
	int bottom = top + (int)blockBoundingRect(block).height();

	while (block.isValid() && top <= event->rect().bottom()) 
	{
		if (bottom >= event->rect().top()) 
		{
			QString number = QString::number(blockNumber + 1);
			painter.setPen(Qt::black);
			painter.drawText(0, top, m_pLineNumberArea->width(), fontMetrics().height(),
				Qt::AlignRight, number);
		}

		block = block.next();
		top = bottom;
		bottom = top + (int)blockBoundingRect(block).height();
		++blockNumber;
	}
}