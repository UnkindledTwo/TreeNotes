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
        if(this->isReadOnly()) return;
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

    QShortcut *find = new QShortcut(QKeySequence("Ctrl+F"), this);
    connect(find, &QShortcut::activated, this, [&](){
        SearchDialog *d = new SearchDialog(this);
        d->show();
    });

    QShortcut *makeHeading = new QShortcut(QKeySequence("Alt+H"), this);
    connect(makeHeading, &QShortcut::activated, this, [&](){
        if(this->isReadOnly()) return;
        QTextCursor c = this->textCursor();
        int pos = c.position();
        c.movePosition(QTextCursor::StartOfBlock);
        c.insertText("# ");
        c.setPosition(pos);
        this->setTextCursor(c);
    });

    QShortcut *makeBold = new QShortcut(QKeySequence("Alt+B"), this);
    connect(makeBold, &QShortcut::activated, this, [&](){
        if(this->isReadOnly()) return;
        QTextCursor c = this->textCursor();
        int selStart = c.selectionStart();
        int selEnd = c.selectionEnd();

        if(selStart == selEnd) return;

        c.setPosition(selStart);
        c.insertText("**");
        c.setPosition(selEnd+2);
        c.insertText("**");
        this->setTextCursor(c);
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
    if(toPlainText().length() > 30000) return;

    int scrollbarpos = this->verticalScrollBar()->value();

    if(!textCursor().selectedText().isEmpty())
        return;

    QTextCharFormat fmt;

    fmt.setBackground(this->palette().color(QPalette::Base));

    QTextCursor c = textCursor();
    int pos = c.position();

    c.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
    c.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    c.setCharFormat(fmt);
    if(lineHighlighting()){
        QColor backColor = this->palette().color(QPalette::Background);
        if(backColor.lightness() > 120){
            fmt.setBackground(backColor.darker(105));
        }
        else{
            fmt.setBackground(backColor.lighter(105));
        }
        //fmt.setBackground(highlightBrush());
    }

    c.setPosition(pos);
    c.movePosition(QTextCursor::StartOfBlock);
    c.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);

    c.setCharFormat(fmt);
    c.setPosition(pos);
    fmt.setBackground(this->palette().color(QPalette::Base));

    if(!symbolHighlighting()) return;

    //Apply highlighting
    for(int i = 0; i < regexVector.count(); i++){
        QRegularExpression reA(regexVector.at(i).regex);
        QRegularExpressionMatchIterator it = reA.globalMatch(toPlainText());
        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            if (match.hasMatch()) {
                //qDebug() << "Match, capturedstart" << match.capturedStart() << "capturedend" << match.capturedEnd();
                c.setPosition(match.capturedStart());
                c.setPosition(match.capturedEnd(), QTextCursor::KeepAnchor);
                //fmt.setBackground(regexVector.at(i).background);
                if(regexVector.at(i).foreground == Qt::black) fmt.setForeground(this->palette().color(QPalette::Foreground));
                else
                    fmt.setForeground(regexVector.at(i).foreground);

                /*
                        if(regexVector.at(i).highlightFontFamily == HighlightFontFamily::Monospace){
                            fmt.setFontFamily(monospaceFontFamily);
                        }
                        */

                if(regexVector.at(i).isBold) fmt.setFontWeight(QFont::Bold);
                if(regexVector.at(i).isItalic) fmt.setFontItalic(true);
                if(regexVector.at(i).isUnderLine) fmt.setUnderlineStyle(QTextCharFormat::SingleUnderline);
                if(regexVector.at(i).isStrikeThrough) fmt.setFontStrikeOut(true);
                else fmt.setUnderlineStyle(QTextCharFormat::NoUnderline);

                //if(regexVector.at(i).fontSize == HighlightFontSize::Double) fmt.setFontPointSize(trunc(this->font().pointSize() * 2));
                //if(regexVector.at(i).highlightFontSize == HighlightFontSize::Half) fmt.setFontPointSize(trunc(this->font().pointSize() / 2));

                c.setCharFormat(fmt);
                fmt.setFont(this->font());
            }
        }
    }


    fmt.merge(c.charFormat());

    this->verticalScrollBar()->setValue(scrollbarpos);
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

    if(e->key() == Qt::Key_Backspace){
        //Check if cursor is inside a pair
        if(pairCompletionMap["" + toPlainText()[textCursor().position()-1]] == ("" + toPlainText()[textCursor().position()])){ // Yuck!
            QTextCursor c = this->textCursor();
            c.deleteChar();
            this->setTextCursor(c);
        }
    }
    if(pairCompletionMap[e->text()] != ""){

        //Check if pair already exists
        /*
        if(pairCompletionMap[e->text()].at(0) == toPlainText()[textCursor().position()]){
            goto noPairCompletion;
        }
        */

        insertPlainText(pairCompletionMap[e->text()]);
        moveCursor(QTextCursor::Left, QTextCursor::MoveAnchor);
    }

noPairCompletion:
    QPlainTextEdit::keyPressEvent(e);

    //highlightCurrentLine();
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

HighlightItem PlainTextEdit::regexVectorItem(
        QString exp,
        QColor fore,
        QColor back,
        bool isBold,
        bool isItalic,
        bool isUnderLine
        ){
    HighlightItem h;
    h.regex = exp;
    h.background = back;
    h.foreground = fore;
    h.isUnderLine = isUnderLine;
    h.isBold = isBold;
    h.isItalic = isItalic;
    /*
    h.highlightFontSize = fs;
    isMonospaced ? h.highlightFontFamily = HighlightFontFamily::Monospace :
            h.highlightFontFamily = HighlightFontFamily::Regular;
            */
    return h;
    //    return QPair<QString, QPair<QColor, QColor>>(exp, QPair<QColor, QColor>(fore, back));
}

void PlainTextEdit::initRegexVector(){
    //regexVector.append(regexVectorItem("(http|https)://[^\\n].*", Qt::blue, Qt::white));
    regexVector.append(regexVectorItem("(http|https)://(\\S|\\t)*",  Qt::blue, Qt::white,  false, false, true));
    regexVector.append(regexVectorItem("\\*{2}.*?\\*{2}", Qt::black, Qt::white, true));
    regexVector.append(regexVectorItem("✔.*",  Qt::darkGreen, Qt::white));
    regexVector.append(regexVectorItem("✖.*", Qt::darkRed, Qt::white));
    regexVector.append(regexVectorItem("λ.*", Qt::blue, Qt::white));
    HighlightItem i = regexVectorItem("~.*~",  Qt::black, Qt::white);
    i.isStrikeThrough = true;
    regexVector.append(i);
    HighlightItem heading = regexVectorItem("# .*",  Qt::darkBlue, Qt::white, true);
    regexVector.append(heading);
    //regexVector.append(regexVectorItem("\\`{1}.*\\`{1}", Qt::black, Qt::lightGray, false, false, false, true));
}

bool PlainTextEdit::lineHighlighting(){
    return m_lineHighlighing;
}

void PlainTextEdit::setLineHighlighting(bool l){
    m_lineHighlighing = l;
    highlightCurrentLine();
    emit lineHighlightingChanged();
}

bool PlainTextEdit::symbolHighlighting()
{
    return m_symbolhighlighting;
}

void PlainTextEdit::setSymbolHighlighting(bool a)
{
    m_symbolhighlighting = a;
    emit symbolHighlightingChanged();
}

void PlainTextEdit::select(int start, int end){
    QTextCursor c = this->textCursor();
    c.setPosition(start);
    c.setPosition(end, QTextCursor::KeepAnchor);
    this->setTextCursor(c);
}

void PlainTextEdit::setPosition(int pos){
    QTextCursor c = this->textCursor();
    if(pos > toPlainText().length() -1){
        pos = toPlainText().length() -1;
    }
    c.setPosition(pos);
    this->setTextCursor(c);
}

void PlainTextEdit::fastAppend(QString m){
    QTextDocument *doc = this->document();
    QTextCursor c(doc);
    c.movePosition(QTextCursor::End);
    //c.beginEditBlock();
    //c.insertBlock();
    c.insertText(m);
    //c.endEditBlock();
}

void PlainTextEdit::fastClear(){
    this->document()->setPlainText("");
}

void PlainTextEdit::fastSetPlainText(QString m){
    this->document()->setPlainText(m);
}
