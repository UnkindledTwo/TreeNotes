#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDebug>
#include <QDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QPlainTextEdit>

#include "plaintextedit.h"
#include "searchresultdialog.h"

namespace Ui {
class SearchDialog;
}

class SearchDialog : public QDialog {
    Q_OBJECT

public:
    void keyPressEvent(QKeyEvent *e);
    explicit SearchDialog(QPlainTextEdit *parent = nullptr);
    ~SearchDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::SearchDialog *ui;
};

#endif  // SEARCHDIALOG_H
