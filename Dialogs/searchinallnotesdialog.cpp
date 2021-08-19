#include "searchinallnotesdialog.h"
#include "ui_searchinallnotesdialog.h"

SearchInAllNotesDialog::SearchInAllNotesDialog(QWidget *parent, TreeWidget *treeWidget) :
    QDialog(parent),
    ui(new Ui::SearchInAllNotesDialog)
{
    ui->setupUi(this);
    this->m_treeWidget = treeWidget;
}

SearchInAllNotesDialog::~SearchInAllNotesDialog()
{
    delete ui;
}

void SearchInAllNotesDialog::on_buttonBox_accepted()
{
    SearchInAllNotesResultDialog *sianrd = new SearchInAllNotesResultDialog(ui->lineEdit->text(), this->m_treeWidget ,ui->checkBox->isChecked(), (QWidget*)this->parent());
    sianrd->setFont(this->font());
    sianrd->show();
    this->close();
}

