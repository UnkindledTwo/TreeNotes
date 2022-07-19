#include "newnotedialog.h"
#include "ui_newnotedialog.h"

NewNoteDialog::NewNoteDialog(TreeWidget* treeWidget, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewNoteDialog)
{
    this->treeWidget = treeWidget;
    ui->setupUi(this);
    ui->b_icon->setIcon(Globals::iconVector[iconVectorIndex]);
}

NewNoteDialog::~NewNoteDialog()
{
    delete ui;
}

void NewNoteDialog::on_buttonBox_accepted()
{
    TreeWidgetItem* twi = treeWidget->createNote(
                ui->le_title->text(),
                "",
                iconVectorIndex);
    if(ui->le_title->text().isEmpty()) twi->setText(0, tr("New Note"));
    twi->highlighting = ui->cb_highlighting->isChecked();
    twi->tags = tags;
    treeWidget->addNote(treeWidget->currentItem(), twi);
    treeWidget->setStar(twi, ui->cb_starred->isChecked());
}

void NewNoteDialog::on_b_icon_clicked()
{
    IconSelectorDialog *isd = new IconSelectorDialog(this, iconVectorIndex);
    isd->copyFrom(this->treeWidget);
    isd->exec();
    iconVectorIndex = isd->index;
    ui->b_icon->setIcon(Globals::iconVector[iconVectorIndex]);
}

void NewNoteDialog::on_b_tags_clicked()
{
    TreeWidgetItem* twi = treeWidget->createNote(ui->le_title->text(), "", iconVectorIndex);
    twi->tags = tags;
    TagEditorDialog* ted = new TagEditorDialog(twi, this);
    if(ted->exec()) {
        tags = twi->tags;
    }
    ui->lbl_tagcnt->setText(QString::number(tags.count()));

    delete twi;
}

