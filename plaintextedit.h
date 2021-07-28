#ifndef PLAINTEXTEDIT_H
#define PLAINTEXTEDIT_H

#include <QWidget>
#include <QDebug>
#include <QPlainTextEdit>
#include <QShortcut>
#include <iostream>
#include <QTextList>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <QRegularExpression>
#include <tuple>
#include <cmath>
#include <QScrollBar>
#include <QtConcurrent>
#include <QTreeWidget>

#include "highlightitem.h"
#include "Dialogs/searchdialog.h"

namespace Ui {
class PlainTextEdit;
}

class PlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT

    Q_PROPERTY(bool pairCompletion READ pairCompletion WRITE setPairCompletion NOTIFY pairCompletionChanged)
    Q_PROPERTY(bool lineHighlighting READ lineHighlighting WRITE setLineHighlighting NOTIFY lineHighlightingChanged)
    Q_PROPERTY(bool symbolHighlighting READ symbolHighlighting WRITE setSymbolHighlighting NOTIFY symbolHighlightingChanged)

public:
    void fastSetPlainText(QString);
    void fastAppend(QString);
    void fastClear();
    void setPosition(int pos);
    void select(int start, int end);
    void paintEvent(QPaintEvent *e);
    void keyPressEvent(QKeyEvent *e);
    explicit PlainTextEdit(QWidget *parent = nullptr);
    ~PlainTextEdit();

    int lineCount();
    bool pairCompletion();
    void setPairCompletion(bool);
    QString lineAt(int);
    int currentLine();
    int currentColumn();

    void highlightCurrentLine();

    bool lineHighlighting();
    void setLineHighlighting(bool l);

    bool symbolHighlighting();
    void setSymbolHighlighting(bool);

signals:
    void pairCompletionChanged();

    void lineHighlightingChanged();
    void symbolHighlightingChanged();
private slots:
    void TextChanged();

    void initPairCompletionMap();
    void initRegexVector();

protected:

private:
    Ui::PlainTextEdit *ui;


    bool m_pairCompletion = true;
    bool m_lineHighlighing = true;
    bool m_symbolhighlighting = true;

    QMap<QString, QString> pairCompletionMap;

    QBrush m_highligtBrush;

    //QVector<QPair<QString, QPair<QColor, QColor>>> regexVector;
    QVector<HighlightItem> regexVector;

    HighlightItem regexVectorItem(QString exp, QColor fore, QColor back = Qt::white, bool isBold = false, bool isItalic = false, bool isUnderLine = false);

    int monospaceFontId = QFontDatabase::addApplicationFont(":/Resources/Fonts/SourceCodePro-Regular.ttf");
    QString monospaceFontFamily = QFontDatabase::applicationFontFamilies(monospaceFontId).at(0);
};

#endif // PLAINTEXTEDIT_H
