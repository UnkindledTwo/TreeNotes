#ifndef SEARCHINALLNOTESRESULTDIALOG_H
#define SEARCHINALLNOTESRESULTDIALOG_H

#include <QDialog>
#include <QTableWidget>

#include "treewidget.h"
#include "plaintextedit.h"

namespace Ui {
class SearchInAllNotesResultDialog;
}

class SearchInAllNotesResultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchInAllNotesResultDialog(QString searchString = "", TreeWidget *widget = NULL, bool caseInsensitive = false, QWidget *parent = nullptr);
    ~SearchInAllNotesResultDialog();

    QTableWidget* table();

    void search();

private slots:
    void on_tableWidget_cellChanged(int row, int column);

    void on_tableWidget_cellDoubleClicked(int row, int column);

private:
    Ui::SearchInAllNotesResultDialog *ui;

    QString searchString;
    QTreeWidget *treeWidget;
    bool caseInsensitive;

    QVector<TreeWidgetItem*> treeItemVector;
};

#endif // SEARCHINALLNOTESRESULTDIALOG_H
