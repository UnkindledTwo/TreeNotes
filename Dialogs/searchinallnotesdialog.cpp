#include "searchinallnotesdialog.h"

#include "ui_searchinallnotesdialog.h"

SearchInAllNotesDialog::SearchInAllNotesDialog(QWidget *parent, TreeWidget *treeWidget)
    : QDialog(parent), ui(new Ui::SearchInAllNotesDialog) {
    ui->setupUi(this);
    this->m_treeWidget = treeWidget;
}

SearchInAllNotesDialog::~SearchInAllNotesDialog() { delete ui; }

void SearchInAllNotesDialog::on_buttonBox_accepted() {
    SearchInAllNotesResultDialog *sianrd =
        new SearchInAllNotesResultDialog(ui->lineEdit->text(),       // Search string
                                         this->m_treeWidget,         // Tree Widget
                                         ui->checkBox->isChecked(),  // Case insensitive
                                         (QWidget *)this->parent()   // Parent
        );
    sianrd->setFont(this->font());
    sianrd->show();
    this->close();
}
