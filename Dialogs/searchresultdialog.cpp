#include "searchresultdialog.h"
#include "ui_searchresultdialog.h"

SearchResultDialog::SearchResultDialog(QPlainTextEdit *pte, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchResultDialog)
{
    ui->setupUi(this);
    this->pte = pte;
}

SearchResultDialog::~SearchResultDialog()
{
    delete ui;
}

QTableWidget* SearchResultDialog::table(){
    return ui->tableWidget;
}

void SearchResultDialog::on_tableWidget_cellDoubleClicked(int row, int column)
{
    table()->item(row, 0);
    QTextCursor c(pte->document()->findBlockByNumber(table()->item(row, 1)->text().toInt()));
    c.setPosition(table()->item(row, 2)->text().toInt());
    c.setPosition(c.position() + table()->item(row, 0)->text().length(), QTextCursor::KeepAnchor);
    pte->setTextCursor(c);
    pte->setFocus();
}


void SearchResultDialog::on_tableWidget_cellChanged(int row, int column)
{
    table()->resizeColumnsToContents();
    ui->resLbl->setText(QString::number(table()->rowCount()) + ((table()->rowCount() > 1) ? " Results" : " Result"));
}

