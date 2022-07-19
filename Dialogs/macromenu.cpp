#include "macromenu.h"
#include "ui_macromenu.h"

MacroMenu::MacroMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MacroMenu)
{
    ui->setupUi(this);

    for (int i = 0; i < Globals::macroVector.count(); i++) {
        ui->listWidget->addItem(Globals::macroVector.at(i).first);
    }
    if(ui->listWidget->count() > 0) {
        ui->listWidget->setCurrentRow(0);
    }
}

MacroMenu::~MacroMenu()
{
    delete ui;
}

void MacroMenu::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Return && ui->listWidget->currentRow() >= 0) {
        emit itemSelected(ui->listWidget->currentRow());
        this->hide();
    }
    else if (e->key() == Qt::Key_Escape) {
        this->hide();
    }
}

void MacroMenu::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    emit itemSelected(ui->listWidget->currentRow());
    this->hide();
}

