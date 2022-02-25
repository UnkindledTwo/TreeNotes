#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <QDebug>
#include <QFontDatabase>
#include <QObject>
#include <QPlainTextEdit>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <QSyntaxHighlighter>
#include <QWidget>

#include "highlightitem.h"

class SyntaxHighlighter : public QSyntaxHighlighter {
public:
    SyntaxHighlighter(QTextDocument *parent, QPlainTextEdit *textEdit);

    QVector<HighlightItem> regexVector;

    QPlainTextEdit *textEdit;

    void setColorfulHighlighting(bool a) {
        m_colorful = a;
        this->rehighlight();
    }
    bool colorfulHighlighting() { return m_colorful; }

protected:
    void highlightBlock(const QString &text) override;

private:
    int monospaceFontId = QFontDatabase::addApplicationFont(":/Resources/Fonts/SourceCodePro-Regular.ttf");
    QString monospaceFontFamily = QFontDatabase::applicationFontFamilies(monospaceFontId).at(0);

    void initRegexVector();

    HighlightItem regexVectorItem(QString exp, QColor fore, QColor back = Qt::white, bool isBold = false, bool isItalic = false,
                                  bool isUnderLine = false, bool isMonospace = false);

    bool m_colorful = true;
};

#endif  // SYNTAXHIGHLIGHTER_H
