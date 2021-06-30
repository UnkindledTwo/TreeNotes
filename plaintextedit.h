#ifndef PLAINTEXTEDIT_H
#define PLAINTEXTEDIT_H

#include <QWidget>
#include <QDebug>
#include <QPlainTextEdit>
#include <QShortcut>
#include <iostream>
#include <QTextList>
#include <QPainter>
#include <QPaintEvent>

namespace Ui {
class PlainTextEdit;
}

class PlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit PlainTextEdit(QWidget *parent = nullptr);
    ~PlainTextEdit();

    int lineCount();

private slots:
protected:
    void paintEvent(QPaintEvent *e);

private:
    Ui::PlainTextEdit *ui;

    void keyPressEvent(QKeyEvent *e);
};

#endif // PLAINTEXTEDIT_H
