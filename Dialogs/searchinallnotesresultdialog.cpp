#include "searchinallnotesresultdialog.h"
#include "ui_searchinallnotesresultdialog.h"

SearchInAllNotesResultDialog::SearchInAllNotesResultDialog(QString searchString, TreeWidget *widget, bool caseInsensitive, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchInAllNotesResultDialog)
{
    ui->setupUi(this);
    this->searchString = searchString;
    this->treeWidget = widget;
    this->caseInsensitive = caseInsensitive;

    this->search();
}

SearchInAllNotesResultDialog::~SearchInAllNotesResultDialog()
{
    delete ui;
}

QTableWidget *SearchInAllNotesResultDialog::table()
{
    return ui->tableWidget;
}

void SearchInAllNotesResultDialog::search()
{
    table()->setRowCount(0);
    treeItemVector.clear();

    QRegularExpression r;
    r.setPattern(this->searchString);

    if(caseInsensitive){
        r.setPatternOptions(r.patternOptions() | r.CaseInsensitiveOption);
    }

    QTreeWidgetItemIterator it(this->treeWidget);

    int row = 0;
    while(*it){
        TreeWidgetItem *item = (TreeWidgetItem*)(*it);
        QRegularExpressionMatch match = r.match(item->message);
        if(match.hasMatch()){
            table()->setRowCount(table()->rowCount()+1);

            QTableWidgetItem *title_ti = new QTableWidgetItem();
            title_ti->setText(item->text(0));
            title_ti->setIcon(item->icon(0));
            table()->setItem(row, 0, title_ti);

            //Get The Block Number By Using a QTextDocument, PlainTextEdit can also be used but is really slow compared to this.
            QTextDocument doc;
            doc.setPlainText(item->message);
            QTextCursor c(&doc);
            c.setPosition(match.capturedStart());
            int pos = c.blockNumber();
            QTableWidgetItem *blockNumber_ti = new QTableWidgetItem();
            blockNumber_ti->setText(QString::number(pos));
            table()->setItem(row, 1, blockNumber_ti);

            QTableWidgetItem *position_ti = new QTableWidgetItem();
            position_ti->setText(QString::number(match.capturedStart()));
            table()->setItem(row, 2, position_ti);

            QTableWidgetItem *starred_ti = new QTableWidgetItem();
            if(item->starred) starred_ti->setIcon(item->icon(1));
            else starred_ti->setText("No");
            table()->setItem(row, 3, starred_ti);

            c.select(QTextCursor::SelectionType::BlockUnderCursor);
            QTableWidgetItem *blockContent_ti = new QTableWidgetItem();
            blockContent_ti->setText(c.selectedText());
            table()->setItem(row, 4, blockContent_ti);


            treeItemVector.append((TreeWidgetItem*)(*it));
            row++;
        }

        it++;
    }

    if(treeItemVector.isEmpty()){
        //No match found
        QMessageBox::warning(this, tr("No Match"), tr("No match found for:\n%1").arg(searchString));
        this->deleteLater();
        return;
    }

    table()->setCurrentItem(table()->item(0,0));
}

void SearchInAllNotesResultDialog::on_tableWidget_cellChanged(int row, int column)
{
    if(column != table()->columnCount() -1)
        table()->resizeColumnToContents(column);
    ui->resLbl->setText(QString::number(table()->rowCount()) + ((table()->rowCount() > 1) ? tr(" Results Found") : tr(" Result Found")));
}


void SearchInAllNotesResultDialog::on_tableWidget_cellDoubleClicked(int row, int column)
{
    this->treeWidget->setCurrentItem(treeItemVector.at(row));
}

