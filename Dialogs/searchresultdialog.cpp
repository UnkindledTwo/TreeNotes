#include "searchresultdialog.h"
#include "ui_searchresultdialog.h"

SearchResultDialog::SearchResultDialog(QPlainTextEdit *pte, QString searchText, bool caseSensitive, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchResultDialog)
{
    ui->setupUi(this);
    this->pte = pte;
    this->searchText = searchText;
    this->caseSensitive = caseSensitive;

    search();
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
    if(column != table()->columnCount() -1)
        table()->resizeColumnToContents(column);
    ui->resLbl->setText(QString::number(table()->rowCount()) + ((table()->rowCount() > 1) ? " Results" : " Result"));
}


void SearchResultDialog::on_bReplace_clicked()
{
    if(table()->currentRow() < 0) return;
    QString replaceWith = QInputDialog::getText(this, "Replace With", "Replace With:");
    qDebug() << replaceWith;

    int row = table()->currentRow();

    QTextCursor c(pte->document()->findBlockByNumber(table()->item(row, 1)->text().toInt()));
    c.setPosition(table()->item(row, 2)->text().toInt());
    c.setPosition(c.position() + table()->item(row, 0)->text().length(), QTextCursor::KeepAnchor);
    c.removeSelectedText();
    c.insertText(replaceWith);
    pte->setTextCursor(c);
    pte->setFocus();

    search();
}

void SearchResultDialog::search()
{
    //table()->clear();
    table()->setRowCount(0);
    QRegularExpression r;
    r.setPattern(this->searchText);

    if(!caseSensitive){
        r.setPatternOptions(r.patternOptions() | r.CaseInsensitiveOption);
    }

    int row = 0;
    QRegularExpressionMatchIterator i(r.globalMatch(pte->toPlainText()));
    if(!i.hasNext()){
        QMessageBox::warning(this, "No Match", "No match found for:\n"+r.pattern());
        this->deleteLater();
        return;
    }
    while(i.hasNext()){
        QRegularExpressionMatch match(i.next());
        if(match.hasMatch()){
            table()->setRowCount(table()->rowCount()+1);
            QTableWidgetItem *i = new QTableWidgetItem();
            i->setText(match.captured());
            table()->setItem(row, 0, i);
            QTextCursor c = pte->textCursor();
            c.setPosition(match.capturedStart());
            int a = c.blockNumber();
            QTableWidgetItem *i1 = new QTableWidgetItem();
            i1->setText(QString::number(a));
            table()->setItem(row, 1, i1);
            QTableWidgetItem *i3 = new QTableWidgetItem();
            i3->setText(QString::number(match.capturedStart()));
            table()->setItem(row, 2, i3);
            QTableWidgetItem *i2 = new QTableWidgetItem();
            c.select(QTextCursor::SelectionType::BlockUnderCursor);
            i2->setText(c.selectedText());
            table()->setItem(row, 3, i2);
            row++;
        }
    }

    table()->setCurrentItem(table()->item(0,0));
    table()->horizontalHeader()->setStretchLastSection(true);
}

