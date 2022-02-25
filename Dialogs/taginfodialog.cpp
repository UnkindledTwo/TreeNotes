#include "taginfodialog.h"

#include "ui_taginfodialog.h"

TagInfoDialog::TagInfoDialog(TreeWidget *tree, QWidget *parent)
    : QDialog(parent), ui(new Ui::TagInfoDialog) {
    this->tree = tree;
    ui->setupUi(this);
    search();
}

TagInfoDialog::~TagInfoDialog() { delete ui; }

void TagInfoDialog::search() {
    ui->tableWidget->setRowCount(0);

    results.clear();

    QStringList tags;

    QTreeWidgetItemIterator it1(tree);
    while (*it1) {
        TreeWidgetItem *currentItem = (TreeWidgetItem *)(*it1);

        tags.append(currentItem->tags);

        it1++;
    }

    QTreeWidgetItemIterator it2(tree);
    while (*it2) {
        TreeWidgetItem *currentItem = (TreeWidgetItem *)(*it2);

        foreach (QString tag, currentItem->tags) { results[tag] = results[tag] + 1; }

        it2++;
    }

    int row = 0;
    QMapIterator<QString, int> it3(results);
    while (it3.hasNext()) {
        it3.next();

        // Title, starred

        ui->tableWidget->setRowCount(row + 1);
        QTableWidgetItem *tagName = new QTableWidgetItem();
        tagName->setText(it3.key());
        ui->tableWidget->setItem(row, 0, tagName);

        QTableWidgetItem *tagCount = new QTableWidgetItem();
        tagCount->setText(QString::number(it3.value()));
        ui->tableWidget->setItem(row, 1, tagCount);

        row++;
    }
}

void TagInfoDialog::on_tableWidget_cellChanged(int row, int column) {
    // Resize changed column if the column is NOT the last column.
    // Resizing the last column makes it look ugly and not stretched
    if (column != ui->tableWidget->columnCount() - 1) {
        ui->tableWidget->resizeColumnToContents(column);
    }

    // Set results label
    ui->resLbl->setText(
        QString::number(ui->tableWidget->rowCount()) +
        ((ui->tableWidget->rowCount() > 1) ? tr(" Results Found") : tr(" Result Found")));
}

void TagInfoDialog::on_tableWidget_cellDoubleClicked(int row, int column) {
    // Search double-clicked tag
    TagResultsDialog *trd = new TagResultsDialog(ui->tableWidget->item(row, 0)->text(), tree, this);
    trd->show();
}
