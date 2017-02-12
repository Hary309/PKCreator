#include "Highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
	: QSyntaxHighlighter(parent)
{
	HighlightingRule rule;

	m_keywordFormat.setForeground(Qt::darkBlue);
	m_keywordFormat.setFontWeight(QFont::Bold);
	
	// keywords
	QStringList keywordPatterns;

	keywordPatterns << "\\band\\b" << "\\bend\\b" << "\\bin\\b" << "\\brepeat\\b" << "\\bwhile\\b" << "\\bbreak\\b"
					<< "\\bfalse\\b" << "\\blocal\\b" << "\\breturn\\b" << "\\bdo\\b" << "\\bfor\\b" << "\\bnil\\b"
					<< "\\bthen\\b" << "\\belse\\b" << "\\bfunction\\b" << "\\bnot\\b" << "\\btrue\\b" << "\\belseif\\b" 
					<< "\\bif\\b" << "\\bor\\b" << "\\buntil\\b";

	for (const QString &pattern : keywordPatterns) 
	{
        rule.pattern = QRegExp(pattern);
        rule.format = m_keywordFormat;
        m_highlightingRules.append(rule);
    }

	m_quotationFormat.setForeground(Qt::darkYellow);
	rule.pattern = QRegExp("\".*\"");
	rule.format = m_quotationFormat;
	m_highlightingRules.append(rule);

	m_functionFormat.setForeground(Qt::darkCyan);
	rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
	rule.format = m_functionFormat;
	m_highlightingRules.append(rule);

	// comments
	m_singleLineCommentFormat.setForeground(Qt::darkGreen);
	rule.pattern = QRegExp("--[^\n]*");
	rule.format = m_singleLineCommentFormat;
	m_highlightingRules.append(rule);

	m_multiLineCommentFormat.setForeground(Qt::darkGreen);

	m_commentStartExpression = QRegExp("--\\[\\[");
	m_commentEndExpression = QRegExp("--\\]\\]");
}

void Highlighter::highlightBlock(const QString &text)
{
	for (const HighlightingRule &rule : m_highlightingRules)
	{
		QRegExp expression(rule.pattern);
		int index = expression.indexIn(text);
		while (index >= 0) 
		{
			int length = expression.matchedLength();
			setFormat(index, length, rule.format);
			index = expression.indexIn(text, index + length);
		}
	}

	setCurrentBlockState(0);

	int startIndex = 0;
	if (previousBlockState() != 1)
		startIndex = m_commentStartExpression.indexIn(text);

	while (startIndex >= 0) 
	{
		int endIndex = m_commentEndExpression.indexIn(text, startIndex);
		int commentLength;

		if (endIndex == -1) 
		{
			setCurrentBlockState(1);
			commentLength = text.length() - startIndex;
		}
		else 
		{
			commentLength = endIndex - startIndex + m_commentEndExpression.matchedLength();
		}

		setFormat(startIndex, commentLength, m_multiLineCommentFormat);
		startIndex = m_commentStartExpression.indexIn(text, startIndex + commentLength);
	}
}
