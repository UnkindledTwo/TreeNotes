#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <QObject>
#include <QWidget>
#include <QSyntaxHighlighter>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <QPlainTextEdit>
#include <QFontDatabase>

#include "highlightitem.h"

class SyntaxHighlighter : public QSyntaxHighlighter
{
public:
    SyntaxHighlighter(QTextDocument* parent, QPlainTextEdit *textEdit);

    QVector<HighlightItem> regexVector;

    QPlainTextEdit *textEdit;
protected:
    void highlightBlock(const QString &text) override;
private:
    int monospaceFontId = QFontDatabase::addApplicationFont(":/Resources/Fonts/SourceCodePro-Regular.ttf");
    QString monospaceFontFamily = QFontDatabase::applicationFontFamilies(monospaceFontId).at(0);

    void initRegexVector();

    HighlightItem regexVectorItem(
            QString exp,
            QColor fore,
            QColor back = Qt::white,
            bool isBold = false,
            bool isItalic = false,
            bool isUnderLine = false,
            bool isMonospace = false
            );
};

#endif // SYNTAXHIGHLIGHTER_H
