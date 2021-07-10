#include "plaintextedit.h"
#include "ui_plaintextedit.h"

PlainTextEdit::PlainTextEdit(QWidget *parent) :
    QPlainTextEdit(parent),
    ui(new Ui::PlainTextEdit)
{
    ui->setupUi(this);
    initRegexVector();
    connect(this, &PlainTextEdit::textChanged, this, &PlainTextEdit::TextChanged);

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

    initPairCompletionMap();
/*
    QTimer *t = new QTimer(this);
    t->setInterval(10);
    connect(t, &QTimer::timeout, this, [&](){
       highlightCurrentLine();
    });
    t->start();*/
    connect(this, &PlainTextEdit::cursorPositionChanged, this, &PlainTextEdit::highlightCurrentLine);


}

void PlainTextEdit::paintEvent(QPaintEvent *e){
    QPlainTextEdit::paintEvent(e);
}

void PlainTextEdit::highlightCurrentLine(){
    if(!textCursor().selectedText().isEmpty())
        return;

    QTextCharFormat fmt;
    fmt.setBackground(Qt::white);
    QTextCursor c = textCursor();
    int pos = c.position();


    c.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
    c.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    c.setCharFormat(fmt);
    fmt.setBackground(highlightBrush());
    c.setPosition(pos);
    c.movePosition(QTextCursor::StartOfBlock, QTextCursor::MoveAnchor);
    c.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);


    c.setCharFormat(fmt);
    c.setPosition(pos);
    //Apply highlighting
    for(int i = 0; i < regexVector.count(); i++){
        QRegularExpression reA(regexVector.at(i).first);
        QRegularExpressionMatchIterator it = reA.globalMatch(toPlainText());
        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            if (match.hasMatch()) {
                //qDebug() << "Match, capturedstart" << match.capturedStart() << "capturedend" << match.capturedEnd();
                c.setPosition(match.capturedStart());
                c.setPosition(match.capturedEnd(), QTextCursor::KeepAnchor);
                fmt.setBackground(regexVector.at(i).second.second);
                fmt.setForeground(regexVector.at(i).second.first);
                c.setCharFormat(fmt);
            }
        }
    }

    c.setCharFormat(fmt);
}

PlainTextEdit::~PlainTextEdit()
{
    delete ui;
}

int PlainTextEdit::lineCount(){
    return this->blockCount();
}

void PlainTextEdit::keyPressEvent(QKeyEvent *e){

    if(!pairCompletion()) goto noPairCompletion;
    if(pairCompletionMap[e->text()] != ""){
        //e->text() = (
        //pairCompletion()[e->text()] = )

        //Check if pair already exists
        if(pairCompletionMap[e->text()].at(0) == toPlainText()[textCursor().position()]){
            goto noPairCompletion;
        }

        insertPlainText(pairCompletionMap[e->text()]);
        moveCursor(QTextCursor::Left, QTextCursor::MoveAnchor);
    }

noPairCompletion:
    QPlainTextEdit::keyPressEvent(e);


}

void PlainTextEdit::TextChanged(){

}

bool PlainTextEdit::pairCompletion(){
    return m_pairCompletion;
}

void PlainTextEdit::setPairCompletion(bool completion){
    m_pairCompletion = completion;
    emit pairCompletionChanged();
}

void PlainTextEdit::initPairCompletionMap(){
    pairCompletionMap["{"] = "}";
    pairCompletionMap["("] = ")";
    pairCompletionMap["["] = "]";
    pairCompletionMap["\""] = "\"";
}

QString PlainTextEdit::lineAt(int line){
    qDebug() << "Calculating line at" << line;
    QString returnVal = "";
    int currentLine = 1;

    for(int i = 0; i < this->toPlainText().length(); i++){
        if(currentLine == line){
            returnVal += this->toPlainText().at(i);
            if(this->toPlainText().at(i) == '\n') currentLine++;

            //Stop the loop if the line interpreted by the loop is higher than the desired line.
            if(currentLine > line) break;
        }
        if(toPlainText().at(i) == '\n') currentLine++;
    }

    qDebug() << "Calculating line at" << line << "finished";
    return returnVal;
}

int PlainTextEdit::currentLine(){
     return textCursor().blockNumber() + 1;
}

int PlainTextEdit::currentColumn(){
    return textCursor().columnNumber() + 1;
}

QBrush PlainTextEdit::highlightBrush(){
    return m_highligtBrush;
}

void PlainTextEdit::setHighlightBrush(QBrush b){
    this->m_highligtBrush = b;
    emit highlightBrushChanged();
}

QPair<QString, QPair<QColor, QColor>> PlainTextEdit::regexVectorItem(QString exp, QColor fore, QColor back){
    return QPair<QString, QPair<QColor, QColor>>(exp, QPair<QColor, QColor>(fore, back));
}

void PlainTextEdit::initRegexVector(){
    regexVector.append(regexVectorItem("(http|https)://.*?\\s", Qt::blue, Qt::white));
}
