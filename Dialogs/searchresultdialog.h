#ifndef SEARCHRESULTDIALOG_H
#define SEARCHRESULTDIALOG_H

#include <QDialog>
#include <QtCore>
#include <QTableWidget>
#include "plaintextedit.h"

namespace Ui {
class SearchResultDialog;
}

class SearchResultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchResultDialog(QPlainTextEdit *pte, QWidget *parent = nullptr);
    //SearchResultDialog(QPlainTextEdit *pte, QWidget *parent);
    ~SearchResultDialog();

    QTableWidget *table();

private slots:
    void on_tableWidget_cellDoubleClicked(int row, int column);
    void on_tableWidget_cellChanged(int row, int column);
private:
    Ui::SearchResultDialog *ui;
    QPlainTextEdit *pte;
};

#endif // SEARCHRESULTDIALOG_H
