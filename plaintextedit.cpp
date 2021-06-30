#include "plaintextedit.h"
#include "ui_plaintextedit.h"

PlainTextEdit::PlainTextEdit(QWidget *parent) :
    QPlainTextEdit(parent),
    ui(new Ui::PlainTextEdit)
{
    ui->setupUi(this);
    this->setCursorWidth(2);

    QShortcut *moveToStart = new QShortcut(QKeySequence("Ctrl+Shift+G"), this);
    connect(moveToStart, &QShortcut::activated,this, [&](){
        qDebug() << "Shortcut";
        QTextCursor textCursor = this->textCursor();
        textCursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
        this->setTextCursor(textCursor);
    });

    QShortcut *moveToEnd = new QShortcut(QKeySequence("Ctrl+G"), this);
    connect(moveToEnd, &QShortcut::activated,this, [&](){
        qDebug() << "Shortcut";
        QTextCursor textCursor = this->textCursor();
        textCursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
        this->setTextCursor(textCursor);
    });

    QShortcut *newLine = new QShortcut(QKeySequence("Ctrl+O"), this);
    connect(newLine, &QShortcut::activated, this, [&](){    qDebug() << "Shortcut";
        QTextCursor textCursor = this->textCursor();
        textCursor.movePosition(QTextCursor::EndOfLine, QTextCursor::MoveAnchor, 1);
        this->setTextCursor(textCursor);
        this->insertPlainText("\n");
    });

    QShortcut *selectLine = new QShortcut(QKeySequence("Ctrl+L"), this);
    connect(selectLine, &QShortcut::activated, this, [&](){
        qDebug() << "Shortcut";
        QTextCursor textCursor = this->textCursor();
        textCursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor, 1);
        this->setTextCursor(textCursor);

        QTextCursor textCursor1 = this->textCursor();
        textCursor1.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor, 1);
        this->setTextCursor(textCursor1);
    });

}

void PlainTextEdit::paintEvent(QPaintEvent *e){

    QPlainTextEdit::paintEvent(e);
}

PlainTextEdit::~PlainTextEdit()
{
    delete ui;
}

int PlainTextEdit::lineCount(){
    int cnt = 0;
    QString text = this->toPlainText();

    for(int i = 0; i < text.length(); i++){
        if(text[i] == '\n'){
            cnt++;
        }
    }

    return cnt + 1;
}

void PlainTextEdit::keyPressEvent(QKeyEvent *e){
    QPlainTextEdit::keyPressEvent(e);
}
