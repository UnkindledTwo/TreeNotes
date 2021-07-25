#include "searchdialog.h"
#include "ui_searchdialog.h"

SearchDialog::SearchDialog(QPlainTextEdit *parent) :
    QDialog(parent),
    ui(new Ui::SearchDialog)
{
    ui->setupUi(this);
    ui->searchBox->setFocus();
    this->adjustSize();
    this->setFixedSize(this->size());
}

SearchDialog::~SearchDialog()
{
    delete ui;
}

void SearchDialog::on_bCancel_clicked()
{
    this->close();
}


void SearchDialog::on_bSearch_clicked()
{
    PlainTextEdit *parent = (PlainTextEdit*)this->parent();
    SearchResultDialog *srd = new SearchResultDialog(parent, parent);

    QRegularExpression r;
    r.setPattern(ui->searchBox->text());

    if(ui->caseInsensitive->isChecked()){
        r.setPatternOptions(r.patternOptions() | r.CaseInsensitiveOption);
    }

    int row = 0;
    QRegularExpressionMatchIterator i(r.globalMatch(parent->toPlainText()));
    while(i.hasNext()){
        QRegularExpressionMatch match(i.next());
        if(match.hasMatch()){
            qDebug() << "Match";
            srd->table()->setRowCount(srd->table()->rowCount()+1);
            QTableWidgetItem *i = new QTableWidgetItem();
            i->setText(match.captured());
            srd->table()->setItem(row, 0, i);
            QTextCursor c = parent->textCursor();
            c.setPosition(match.capturedStart());
            int a = c.blockNumber();
            QTableWidgetItem *i1 = new QTableWidgetItem();
            i1->setText(QString::number(a));
            srd->table()->setItem(row, 1, i1);
            QTableWidgetItem *i3 = new QTableWidgetItem();
            i3->setText(QString::number(match.capturedStart()));
            srd->table()->setItem(row, 2, i3);
            QTableWidgetItem *i2 = new QTableWidgetItem();
            c.select(QTextCursor::SelectionType::BlockUnderCursor);
            i2->setText(c.selectedText());
            srd->table()->setItem(row, 3, i2);
            row++;
        }
    }

    this->close();
    srd->setFont(this->font());
    srd->exec();

}

void SearchDialog::keyPressEvent(QKeyEvent *e){
    if(e->key() == Qt::Key_Return){
        on_bSearch_clicked();
    }
}

