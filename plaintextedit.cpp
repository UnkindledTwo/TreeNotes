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
    connect(this, &PlainTextEdit::cursorPositionChanged, this, &PlainTextEdit::highlightCurrentLine);
}

void PlainTextEdit::paintEvent(QPaintEvent *e){
    QPlainTextEdit::paintEvent(e);
}

void PlainTextEdit::highlightCurrentLine(){
    if(toPlainText().length() > 30000) return;
    if(!textCursor().selectedText().isEmpty())
        return;

    QColor backColor = this->palette().color(QPalette::Background);
    bool darkTheme = backColor.lightness() < 150;
    int scrollbarpos = this->verticalScrollBar()->value();


    QTextCursor c = textCursor();
    int pos = c.position();
    QTextCharFormat fmt;
    if(!lineHighlighting())
        goto noLineHighlight;

    fmt.setBackground(this->palette().color(QPalette::Base));



    c.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
    c.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    c.setCharFormat(fmt);
    if(lineHighlighting()){
        if(!darkTheme){
            fmt.setBackground(backColor.darker(115));
        }
        else{
            fmt.setBackground(backColor.lighter(125));
        }
    }

noLineHighlight:

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
                if(regexVector.at(i).foreground == Qt::black) fmt.setForeground(this->palette().color(QPalette::Foreground));
                else
                    if(darkTheme)
                        fmt.setForeground(regexVector.at(i).foreground.lighter(155));
                    else
                        fmt.setForeground(regexVector.at(i).foreground.darker(105));

                //h1
                if(regexVector.at(i).regex == "(?m)^#{1}(?!#)(.*)") {
                    fmt.setFontPointSize(this->font().pointSizeF() * 1.72);
                }
                //h2
                if(regexVector.at(i).regex == "(?m)^#{2}(?!#)(.*)") {
                    fmt.setFontPointSize(this->font().pointSizeF() * 1.44);
                }
                //h3
                if(regexVector.at(i).regex == "(?m)^#{3}(?!#)(.*)") {
                    fmt.setFontPointSize(this->font().pointSizeF() * 1.20);
                }
                //h4
                if(regexVector.at(i).regex == "(?m)^#{4}(?!#)(.*)") {
                    fmt.setFontPointSize(this->font().pointSizeF() * 1.15);
                }
                //h5
                if(regexVector.at(i).regex == "(?m)^#{5}(?!#)(.*)") {
                    fmt.setFontPointSize(this->font().pointSizeF() * 1.10);
                }
                //h6
                if(regexVector.at(i).regex == "(?m)^#{6}(?!#)(.*)") {
                    fmt.setFontPointSize(this->font().pointSizeF() * 1.05);
                }

                if(regexVector.at(i).isBold) fmt.setFontWeight(QFont::Bold);
                if(regexVector.at(i).isItalic) fmt.setFontItalic(true);
                if(regexVector.at(i).isStrikeThrough) fmt.setFontStrikeOut(true);
                if(regexVector.at(i).isMonospace) {
                    fmt.setFontFamily(monospaceFontFamily);
                }
                if(regexVector.at(i).isUnderLine) fmt.setUnderlineStyle(QTextCharFormat::SingleUnderline);
                else fmt.setUnderlineStyle(QTextCharFormat::NoUnderline);

                c.setCharFormat(fmt);
                fmt.setFont(this->font());
            }
        }
    }

    fmt.merge(c.charFormat());
    this->verticalScrollBar()->setValue(scrollbarpos);

    //Fix for scrollbars
    //When something with a different font size is added to the bottom of the document.
    //Scrolling with the arrow keys will break this widget
    //Not sure if this fixes it. But here you go.
    c.setPosition(pos);
    this->setTextCursor(c);
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

        //Check if there is text selected
        if(textCursor().selectedText().length() == 0){
            insertPlainText(pairCompletionMap[e->text()]);
            moveCursor(QTextCursor::Left, QTextCursor::MoveAnchor);
        }
        else{
            QTextCursor c = this->textCursor();
            //Save the end and start indices of selection
            int selEnd = c.selectionEnd(), selStart = c.selectionStart();
            QString selectedText = c.selectedText();
            c.removeSelectedText();
            c.setPosition(selStart);
            c.insertText(e->text());
            c.insertText(selectedText);
            c.insertText(pairCompletionMap[e->text()]);
            this->setTextCursor(c);
            goto cancelKeyPress;
        }
    }

noPairCompletion:
    QPlainTextEdit::keyPressEvent(e);
cancelKeyPress:
    ;
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

HighlightItem PlainTextEdit::regexVectorItem(
        QString exp,
        QColor fore,
        QColor back,
        bool isBold,
        bool isItalic,
        bool isUnderLine,
        bool isMonospace
        ){
    HighlightItem h;
    h.regex = exp;
    h.background = back;
    h.foreground = fore;
    h.isUnderLine = isUnderLine;
    h.isBold = isBold;
    h.isItalic = isItalic;
    h.isMonospace = isMonospace;
    return h;
}

void PlainTextEdit::initRegexVector(){
    //Links
    regexVector.append(regexVectorItem("(http[s]?:\\/\\/(www\\.)?|ftp:\\/\\/(www\\.)?|www\\.){1}([0-9A-Za-z-\\.@:%_\+~#=]+)+((\\.[a-zA-Z]{2,3})+)(/(.)*)?(\\?(.)*)?",  Qt::blue, Qt::white,  false, false, true));

    //Italic
    regexVector.append(regexVectorItem("\\*[A-z0-9]+\\*", Qt::black, Qt::white, false, true));
    //Bold
    regexVector.append(regexVectorItem("\\*\\*[A-z0-9]+\\*\\*", Qt::black, Qt::white, true));
    //BoldItalic
    regexVector.append(regexVectorItem("\\*\\*\\*[A-z0-9]+\\*\\*\\*", Qt::black, Qt::white, true, true));

    //Monospace
    regexVector.append(regexVectorItem("`.*`", Qt::black, Qt::white, false, false, false, true));

    regexVector.append(regexVectorItem("✔((?!✖).)*",  Qt::darkGreen, Qt::white));
    regexVector.append(regexVectorItem("✖((?!✔).)*", Qt::darkRed, Qt::white));
    //Lambda is broken
    regexVector.append(regexVectorItem("λ.*", QColor(8, 129, 199), Qt::white));
    HighlightItem i = regexVectorItem("\\~[A-z0-9]+\\~",  Qt::black, Qt::white);
    i.isStrikeThrough = true;
    regexVector.append(i);
    //Headings through 1 to 6
    for(int i = 1; i < 7; i++) {
        HighlightItem heading = regexVectorItem(QString("(?m)^#{%1}(?!#)(.*)").arg(QString::number(i)), Qt::darkBlue, Qt::white, true);
        regexVector.append(heading);
    }
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
    c.insertText(m);
}

void PlainTextEdit::fastClear(){
    this->document()->setPlainText("");
}

void PlainTextEdit::search()
{
    SearchDialog *d = new SearchDialog(this);
    d->show();
}

void PlainTextEdit::replaceAll()
{
    bool lookForOk;
    QString lookFor = QInputDialog::getText(
                this,
                "Replace All",
                "Look For:",
                QLineEdit::Normal,
                this->textCursor().selectedText(),
                &lookForOk
                );
    if(!lookForOk) return;

    bool replaceWithOk;
    QString replaceWith = QInputDialog::getText(
                this,
                "Replace All",
                "Replace With:",
                QLineEdit::Normal,
                "",
                &replaceWithOk
                );
    if(lookFor.isEmpty() || !replaceWithOk) return;

    setPlainText(toPlainText().replace(lookFor, replaceWith));

}

void PlainTextEdit::fastSetPlainText(QString m){
    this->document()->setPlainText(m);
}
