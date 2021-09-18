#include "plaintextedit.h"
#include "ui_plaintextedit.h"

PlainTextEdit::PlainTextEdit(QWidget *parent) :
    QPlainTextEdit(parent),
    ui(new Ui::PlainTextEdit)
{
    ui->setupUi(this);
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

    QShortcut *showTabsAndSpaces = new QShortcut(QKeySequence("Alt+T"), this);
    connect(showTabsAndSpaces, &QShortcut::activated, this, [&](){
        QTextOption op;
        op.setFlags(QTextOption::ShowTabsAndSpaces);
        if(this->document()->defaultTextOption().flags() == QTextOption::ShowTabsAndSpaces) {
            this->document()->setDefaultTextOption(QTextOption());
        }
        else {
            this->document()->setDefaultTextOption(op);
        }
    });

    initPairCompletionMap();

    //Create the syntax highlighter
    high = new SyntaxHighlighter(this->document(), this /*Text Editor (QPlainTextEdit)*/);

    //Resize headings when zoom changes
    connect(this, &PlainTextEdit::zoomChanged, high, &SyntaxHighlighter::rehighlight);
}

void PlainTextEdit::paintEvent(QPaintEvent *e){
    //You NEED to call painter on viewport or it doesn't work
    //QPainter painter(this->viewport());

    QPlainTextEdit::paintEvent(e);
}

void PlainTextEdit::changeEvent(QEvent *e)
{
    //Handle style and font change
    switch(e->type()) {
    case QEvent::StyleChange:
    case QEvent::FontChange:
        high->rehighlight();
        break;
    }

    QPlainTextEdit::changeEvent(e);
}

PlainTextEdit::~PlainTextEdit()
{
    delete ui;
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
            int selStart = c.selectionStart();
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
    return this->document()->findBlockByNumber(line).text();
}

int PlainTextEdit::currentLine(){
    return textCursor().blockNumber() + 1;
}

int PlainTextEdit::currentColumn(){
    return textCursor().columnNumber() + 1;
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

void PlainTextEdit::setZoomingEnabled(bool)
{
    this->m_zooming_enabled = this;
}

bool PlainTextEdit::zoomingEnabled()
{
    return this->m_zooming_enabled;
}

void PlainTextEdit::wheelEvent(QWheelEvent *e)
{
    if(e->modifiers() == Qt::ControlModifier && this->zoomingEnabled()) {
        if(e->delta() > 0) {
            this->zoomIn();
            emit this->zoomChanged();
        }
        else if(e->delta() < 0) {
            this->zoomOut();
            emit this->zoomChanged();
        }
    }
    else
        QPlainTextEdit::wheelEvent(e);
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
