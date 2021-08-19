#ifndef SEARCHINALLNOTESDIALOG_H
#define SEARCHINALLNOTESDIALOG_H

#include <QDialog>
#include <QTableWidget>

#include "Dialogs/searchinallnotesresultdialog.h"
#include "treewidget.h"

namespace Ui {
class SearchInAllNotesDialog;
}

class SearchInAllNotesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchInAllNotesDialog(QWidget *parent = nullptr, TreeWidget *treeWidget = NULL);
    ~SearchInAllNotesDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::SearchInAllNotesDialog *ui;

    TreeWidget *m_treeWidget;
};

#endif // SEARCHINALLNOTESDIALOG_H
