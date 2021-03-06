#include "macrohelp.h"

#include "ui_macrohelp.h"

MacroHelp::MacroHelp(QWidget *parent)
    : QDialog(parent), ui(new Ui::MacroHelp) {
    ui->setupUi(this);
    for (int i = 0; i < Globals::macroVector.count(); i++) {
        ui->macrolist->addItem(Globals::macroVector.at(i).first);
    }

    // Put the list widget and text into a QSplitter
    QSplitter *splitter = new QSplitter();
    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setSpacing(0);
    vbox->setMargin(0);
    vbox->addWidget(ui->label);
    vbox->addWidget(ui->macrolist);
    QWidget *wrapper = new QWidget();
    wrapper->setLayout(vbox);
    splitter->addWidget(wrapper);
    splitter->addWidget(ui->plainTextEdit);
    ((QGridLayout *)this->layout())->addWidget(splitter);

    this->layout()->setMargin(parent->layout()->margin());
    this->setFont(parent->font());
}

MacroHelp::~MacroHelp() { delete ui; }

QListWidget *MacroHelp::listWidget() { return ui->macrolist; }

QPlainTextEdit *MacroHelp::plainTextEdit() { return ui->plainTextEdit; }
