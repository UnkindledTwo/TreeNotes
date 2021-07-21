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

    QRegularExpression r;
    r.setPattern(ui->searchBox->text());

    if(ui->caseInsensitive->isChecked()){
        r.setPatternOptions(r.patternOptions() | r.CaseInsensitiveOption);
    }

    QRegularExpressionMatch match = r.match(parent->toPlainText());
    if(match.hasMatch()){
        qDebug() << "Match";
        parent->select(match.capturedStart(), match.capturedEnd());
        this->close();
        parent->setFocus();
    }

}

void SearchDialog::keyPressEvent(QKeyEvent *e){
    if(e->key() == Qt::Key_Return){
        on_bSearch_clicked();
    }
}

