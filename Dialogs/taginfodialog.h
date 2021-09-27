#ifndef TAGINFODIALOG_H
#define TAGINFODIALOG_H

#include <QDialog>
#include <QMap>

#include "treewidget.h"
#include "Dialogs/tagresultsdialog.h"

namespace Ui {
class TagInfoDialog;
}

class TagInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TagInfoDialog(TreeWidget *tree, QWidget *parent = nullptr);
    ~TagInfoDialog();

private slots:
    void on_tableWidget_cellChanged(int row, int column);

    void on_tableWidget_cellDoubleClicked(int row, int column);

private:
    Ui::TagInfoDialog *ui;

    void search();

    QMap<QString, int> results;
    TreeWidget *tree;
};

#endif // TAGINFODIALOG_H
