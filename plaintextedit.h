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

    Q_PROPERTY(bool pairCompletion READ pairCompletion WRITE setPairCompletion NOTIFY pairCompletionChanged)

public:
    explicit PlainTextEdit(QWidget *parent = nullptr);
    ~PlainTextEdit();

    int lineCount();
    bool pairCompletion();
    void setPairCompletion(bool);
    QString lineAt(int);
    int currentLine();
    int currentColumn();

signals:
    void pairCompletionChanged();
private slots:
    void TextChanged();

    void initPairCompletionMap();

protected:
    void paintEvent(QPaintEvent *e);

private:
    Ui::PlainTextEdit *ui;

    void keyPressEvent(QKeyEvent *e);

    bool m_pairCompletion = true;

    QMap<QString, QString> pairCompletionMap;

};

#endif // PLAINTEXTEDIT_H
