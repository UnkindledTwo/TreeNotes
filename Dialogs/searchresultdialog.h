#ifndef SEARCHRESULTDIALOG_H
#define SEARCHRESULTDIALOG_H

#include <QDialog>
#include <QtCore>
#include <QTableWidget>
#include <QInputDialog>
#include "plaintextedit.h"

namespace Ui {
class SearchResultDialog;
}

class SearchResultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchResultDialog(QPlainTextEdit *pte, QString searchText = "", bool caseSensitive = true, QWidget *parent = nullptr);
    //SearchResultDialog(QPlainTextEdit *pte, QWidget *parent);
    ~SearchResultDialog();

    QTableWidget *table();

private slots:
    void on_tableWidget_cellDoubleClicked(int row, int column);
    void on_tableWidget_cellChanged(int row, int column);
    void on_bReplace_clicked();

private:
    Ui::SearchResultDialog *ui;
    QPlainTextEdit *pte;

    QString searchText = "";
    bool caseSensitive = true;
    void search();
};

#endif // SEARCHRESULTDIALOG_H
