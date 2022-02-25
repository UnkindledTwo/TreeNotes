#include "scratchpad.h"

#include "ui_scratchpad.h"

ScratchPad::ScratchPad(QWidget *parent) : QMainWindow(parent), ui(new Ui::ScratchPad) {
    ui->setupUi(this);
    textEdit = new PlainTextEdit(this);
    this->setCentralWidget(textEdit);
    createStatusLabels();
    connect(textEdit, &PlainTextEdit::textChanged, this, &ScratchPad::updateStatusLabels);
    connect(textEdit, &PlainTextEdit::cursorPositionChanged, this, &ScratchPad::updateStatusLabels);
}

ScratchPad::~ScratchPad() { delete ui; }

void ScratchPad::createStatusLabels() {
    l_length = new QLabel();
    l_col = new QLabel();
    l_row = new QLabel();
    l_readOnly = new QLabel();
    l_readOnly->setText(QString("<b>%1</b>").arg(tr("Read Only")));
    l_readOnly->hide();

    ui->statusbar->addWidget(l_length);
    ui->statusbar->addWidget(l_row);
    ui->statusbar->addWidget(l_col);
    ui->statusbar->addWidget(l_readOnly);

    updateStatusLabels();
}

void ScratchPad::updateStatusLabels() {
    this->l_length->setText(tr("length: ") + QString::number(textEdit->toPlainText().length()));
    this->l_col->setText(tr("col: ") + QString::number(textEdit->currentColumn()));
    this->l_row->setText(tr("row: ") + QString::number(textEdit->currentLine()));
}

void ScratchPad::closeEvent(QCloseEvent *e) {
    e->accept();
    emit closed();
}

void ScratchPad::on_actionDelete_Scratch_Pad_triggered() {
    this->close();
    emit deleteRequested(this);
}

void ScratchPad::on_actionRead_Only_triggered(bool checked) {
    textEdit->setReadOnly(checked);
    l_readOnly->setHidden(!checked);
}
