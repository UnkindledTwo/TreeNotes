#include "searchdialog.h"

#include "ui_searchdialog.h"

SearchDialog::SearchDialog(QPlainTextEdit *parent) : QDialog(parent), ui(new Ui::SearchDialog) {
    ui->setupUi(this);
    ui->searchBox->setFocus();
}

SearchDialog::~SearchDialog() { delete ui; }

void SearchDialog::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Return) {
        // Accept
        on_buttonBox_accepted();
    }
    if (e->key() == Qt::Key_Escape) {
        // Reject
        on_buttonBox_rejected();
    }
}

void SearchDialog::on_buttonBox_accepted() {
    if (ui->searchBox->text().isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Search String <b>Can't</b> Be Empty"));
        return;
    }
    PlainTextEdit *parent = (PlainTextEdit *)this->parent();
    SearchResultDialog *srd = new SearchResultDialog(parent, ui->searchBox->text(),
                                                     !ui->caseInsensitive->isChecked(), parent);

    this->close();
    srd->setFont(this->font());
    srd->show();
    this->accept();
}

void SearchDialog::on_buttonBox_rejected() {
    this->reject();
    this->close();
}
