#include "searchdialog.h"
#include "ui_searchdialog.h"

SearchDialog::SearchDialog(QPlainTextEdit *parent) :
    QDialog(parent),
    ui(new Ui::SearchDialog)
{
    ui->setupUi(this);
    ui->searchBox->setFocus();
    this->setStyleSheet("");
    this->setStyleSheet("QLabel{border : 0px solid; } QCheckBox{border : 0px solid;} QLineEdit{font-size: 16px;} QPushButton:hover{background-color: white;}");
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
    if(ui->caseSensitive->isChecked())
        r.setPattern(ui->searchBox->text());
    else{
        r.setPattern("(?i)"+ui->searchBox->text());
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

