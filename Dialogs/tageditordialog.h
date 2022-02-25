#ifndef TAGEDITORDIALOG_H
#define TAGEDITORDIALOG_H

#include <QDialog>

#include "treewidgetitem.h"

namespace Ui {
class TagEditorDialog;
}

class TagEditorDialog : public QDialog {
    Q_OBJECT

public:
    explicit TagEditorDialog(TreeWidgetItem *item, QWidget *parent = nullptr);
    ~TagEditorDialog();
private slots:
    void readTags();
    void saveTags();

private:
    TreeWidgetItem *item;

    Ui::TagEditorDialog *ui;
};

#endif  // TAGEDITORDIALOG_H
