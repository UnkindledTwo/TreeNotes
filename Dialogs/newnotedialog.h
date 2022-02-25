#pragma once

#include <QDialog>

#include "treewidget.h"
#include "iconselectordialog.h"
#include "tageditordialog.h"

namespace Ui {
class NewNoteDialog;
}

class NewNoteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewNoteDialog(TreeWidget* treeWidget, QWidget *parent = nullptr);
    ~NewNoteDialog();

private slots:
    void on_buttonBox_accepted();

    void on_b_icon_clicked();

    void on_b_tags_clicked();

private:
    TreeWidget* treeWidget;
    int iconVectorIndex = 0;
    QStringList tags;

    Ui::NewNoteDialog *ui;
};

