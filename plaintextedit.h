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
#include <QStack>

#include "highlightitem.h"
#include "Dialogs/searchdialog.h"
#include "syntaxhighlighter.h"

namespace Ui {
class PlainTextEdit;
}

class PlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT

    Q_PROPERTY(bool pairCompletion READ pairCompletion WRITE setPairCompletion NOTIFY pairCompletionChanged)
public:
    void setTabStopWidth(int width);

    void setZoomingEnabled(bool);
    bool zoomingEnabled();

    void search();
    void replaceAll();

    void fastSetPlainText(QString);
    void fastAppend(QString);
    void fastClear();
    void setPosition(int pos);
    void select(int start, int end);

    void wheelEvent(QWheelEvent *e);
    void keyPressEvent(QKeyEvent *e);
    explicit PlainTextEdit(QWidget *parent = nullptr);
    ~PlainTextEdit();

    int lineCount();
    bool pairCompletion();
    void setPairCompletion(bool);
    QString lineAt(int);
    int currentLine();
    int currentColumn();

protected:
    void paintEvent(QPaintEvent *e);
    void changeEvent(QEvent *e);
signals:
    void zoomChanged();

    void pairCompletionChanged();

private slots:
    void TextChanged();

    void initPairCompletionMap();
protected:

private:
    int tabWidth = 4;
    Ui::PlainTextEdit *ui;

    SyntaxHighlighter *high;

    bool m_pairCompletion = true;
    bool m_zooming_enabled = false;

    QMap<QString, QString> pairCompletionMap;
};

#endif // PLAINTEXTEDIT_H
