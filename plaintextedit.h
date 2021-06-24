#ifndef PLAINTEXTEDIT_H
#define PLAINTEXTEDIT_H

#include <QWidget>
#include <QDebug>
#include <QPlainTextEdit>
#include <QShortcut>
#include <iostream>
#include <QTextList>

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
private:
    Ui::PlainTextEdit *ui;

    void keyPressEvent(QKeyEvent *e);
};

#endif // PLAINTEXTEDIT_H
