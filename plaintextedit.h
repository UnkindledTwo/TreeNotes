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
#include "highlightitem.h"

namespace Ui {
class PlainTextEdit;
}

class PlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT

    Q_PROPERTY(bool pairCompletion READ pairCompletion WRITE setPairCompletion NOTIFY pairCompletionChanged)
    Q_PROPERTY(QBrush highlightBrush READ highlightBrush WRITE setHighlightBrush NOTIFY highlightBrushChanged)

public:
    void paintEvent(QPaintEvent *e);
    explicit PlainTextEdit(QWidget *parent = nullptr);
    ~PlainTextEdit();

    int lineCount();
    bool pairCompletion();
    void setPairCompletion(bool);
    QString lineAt(int);
    int currentLine();
    int currentColumn();

    void highlightCurrentLine();

    QBrush highlightBrush();
    void setHighlightBrush(QBrush b);

signals:
    void pairCompletionChanged();

    void highlightBrushChanged();
private slots:
    void TextChanged();

    void initPairCompletionMap();
    void initRegexVector();

protected:

private:
    Ui::PlainTextEdit *ui;

    void keyPressEvent(QKeyEvent *e);

    bool m_pairCompletion = true;

    QMap<QString, QString> pairCompletionMap;

    QBrush m_highligtBrush;

    //QVector<QPair<QString, QPair<QColor, QColor>>> regexVector;
    QVector<HighlightItem> regexVector;

    HighlightItem regexVectorItem(QString exp, QColor fore, QColor back = Qt::white, HighlightFontSize fontSize = HighlightFontSize::Same, bool isBold = false, bool isItalic = false, bool isUnderLine = false);

};

#endif // PLAINTEXTEDIT_H
