#pragma once

#include <QSyntaxHighlighter>

class Highlighter : public QSyntaxHighlighter
{
	//Q_OBJECT

private:
	struct HighlightingRule
	{
		QRegExp pattern;
		QTextCharFormat format;
	};

	QVector<HighlightingRule>	m_highlightingRules;

	QRegExp						m_commentStartExpression;
	QRegExp						m_commentEndExpression;

	QTextCharFormat				m_keywordFormat;
	QTextCharFormat				m_singleLineCommentFormat;
	QTextCharFormat				m_multiLineCommentFormat;
	QTextCharFormat				m_quotationFormat;
	QTextCharFormat				m_functionFormat;

protected:
	void highlightBlock(const QString &text) override;

public:
	Highlighter(QTextDocument *parent = 0);
};
