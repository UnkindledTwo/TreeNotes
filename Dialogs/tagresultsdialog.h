#ifndef TAGRESULTSDIALOG_H
#define TAGRESULTSDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QTableWidgetItem>

#include "treewidget.h"

namespace Ui {
class TagResultsDialog;
}

class TagResultsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TagResultsDialog(QString tag, TreeWidget *tree, QWidget *parent = nullptr);
    ~TagResultsDialog();

private slots:
    void on_tableWidget_cellChanged(int row, int column);

    void on_tableWidget_cellDoubleClicked(int row, int column);

private:
    Ui::TagResultsDialog *ui;

    void search();

    QVector<TreeWidgetItem*> found;

    QString tag;
    TreeWidget *tree;
};

#endif // TAGRESULTSDIALOG_H
