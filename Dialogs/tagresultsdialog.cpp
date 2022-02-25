#include "tagresultsdialog.h"

#include "ui_tagresultsdialog.h"

TagResultsDialog::TagResultsDialog(QString tag, TreeWidget* tree, QWidget* parent)
    : QDialog(parent), ui(new Ui::TagResultsDialog) {
    this->tag = tag;
    this->tree = tree;
    ui->setupUi(this);

    this->search();
    this->setWindowTitle(this->windowTitle() + " - " + tag);
}

TagResultsDialog::~TagResultsDialog() { delete ui; }

void TagResultsDialog::search() {
    ui->tableWidget->setRowCount(0);

    found.clear();
    QTreeWidgetItemIterator it(tree);
    while (*it) {
        TreeWidgetItem* currentItem = (TreeWidgetItem*)(*it);

        if (currentItem->tags.indexOf(tag) >= 0) {
            found.append(currentItem);
        }

        it++;
    }

    int row = 0;
    foreach (TreeWidgetItem* item, found) {
        // Title,
        // starred

        ui->tableWidget->setRowCount(row + 1);
        QTableWidgetItem* title = new QTableWidgetItem();
        title->setText(item->text(0));
        title->setIcon(item->icon(0));
        ui->tableWidget->setItem(row, 0, title);

        QTableWidgetItem* starred = new QTableWidgetItem();
        if (item->starred) {
            starred->setIcon(item->icon(1));
        } else {
            starred->setText("No");
        }
        ui->tableWidget->setItem(row, 1, starred);

        row++;
    }
}

void TagResultsDialog::on_tableWidget_cellChanged(int row, int column) {
    // Resize changed column if the column is NOT the last column.
    // Resizing the last column makes it look ugly and not stretched
    ui->tableWidget->resizeColumnToContents(column);

    // Set
    // results
    // label
    ui->resLbl->setText(
        QString::number(ui->tableWidget->rowCount()) +
        ((ui->tableWidget->rowCount() > 1) ? tr(" Results Found") : tr(" Result Found")));

    ui->tableWidget->sortByColumn(1, Qt::DescendingOrder);
}

void TagResultsDialog::on_tableWidget_cellDoubleClicked(int row, int column) {
    // Select
    // the
    // selected
    // item in
    // the
    // table
    tree->setCurrentItem(found.at(row));
}
