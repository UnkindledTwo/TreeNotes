#include "tageditordialog.h"

#include "ui_tageditordialog.h"

TagEditorDialog::TagEditorDialog(TreeWidgetItem *item, QWidget *parent)
    : QDialog(parent), ui(new Ui::TagEditorDialog) {
    this->item = item;
    ui->setupUi(this);
    ui->plainTextEdit->setZoomingEnabled(true);
    ui->plainTextEdit->zoomIn(2);
    ui->plainTextEdit->setHighlighting(false);

    readTags();

    connect(this, &TagEditorDialog::accepted, this, &TagEditorDialog::saveTags);
}

TagEditorDialog::~TagEditorDialog() { delete ui; }

void TagEditorDialog::readTags() {
    foreach (QString tag, item->tags) { ui->plainTextEdit->fastAppend(tag + '\n'); }
}

void TagEditorDialog::saveTags() {
    item->tags.clear();
    QStringList tags = ui->plainTextEdit->toPlainText().split('\n');
    tags.removeDuplicates();
    foreach (QString tag, tags) {
        if (tag == "\n" || tag == " " || tag.isEmpty()) continue;
        item->tags.append(tag.trimmed());
    }
}
