#include "syntaxhighlighter.h"

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent, QPlainTextEdit *textEdit)
    : QSyntaxHighlighter(parent) {
    this->textEdit = textEdit;
    initRegexVector();
    this->setDocument(parent);
}

void SyntaxHighlighter::highlightBlock(const QString &text) {
    QColor backColor = this->textEdit->palette().color(QPalette::Background);

    // Decide if the user is using the dark theme
    bool darkTheme = backColor.lightness() < 150;

    QColor highlightColor;
    if (!darkTheme) {
        highlightColor = QColor(backColor.darker(115));
    } else {
        highlightColor = QColor(backColor.lighter(125));
    }

    foreach (HighlightItem highlightItem, this->regexVector) {
        QRegularExpression reA(highlightItem.regex);
        QRegularExpressionMatchIterator it = reA.globalMatch(text);
        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            // if(match.hasMatch()){
            QTextCharFormat fmt;

            // qDebug() << "Match, capturedstart" << match.capturedStart() << "capturedend" <<
            // match.capturedEnd();
            if (!colorfulHighlighting()) goto noColor;
            if (highlightItem.foreground == Qt::black)
                fmt.setForeground(this->textEdit->palette().color(QPalette::Foreground));
            else if (darkTheme)
                // 155
                fmt.setForeground(highlightItem.foreground.lighter(185));
            else
                fmt.setForeground(highlightItem.foreground.darker(105));

        noColor:

            // h1
            if (highlightItem.regex == "(?m)^#{1} (?!#)(.*)") {
                fmt.setFontPointSize(this->textEdit->font().pointSizeF() * 1.72);
            }
            // h2
            if (highlightItem.regex == "(?m)^#{2} (?!#)(.*)") {
                fmt.setFontPointSize(this->textEdit->font().pointSizeF() * 1.44);
            }
            // h3
            if (highlightItem.regex == "(?m)^#{3} (?!#)(.*)") {
                fmt.setFontPointSize(this->textEdit->font().pointSizeF() * 1.20);
            }
            // h4
            if (highlightItem.regex == "(?m)^#{4} (?!#)(.*)") {
                fmt.setFontPointSize(this->textEdit->font().pointSizeF() * 1.15);
            }
            // h5
            if (highlightItem.regex == "(?m)^#{5} (?!#)(.*)") {
                fmt.setFontPointSize(this->textEdit->font().pointSizeF() * 1.10);
            }
            // h6
            if (highlightItem.regex == "(?m)^#{6} (?!#)(.*)") {
                fmt.setFontPointSize(this->textEdit->font().pointSizeF() * 1.05);
            }

            if (highlightItem.isBold) fmt.setFontWeight(QFont::Bold);
            if (highlightItem.isItalic) fmt.setFontItalic(true);
            if (highlightItem.isStrikeThrough) fmt.setFontStrikeOut(true);
            if (highlightItem.isMonospace) {
                fmt.setAnchor(true);
                fmt.setAnchorHref("aspıodjapısdj");
                fmt.setAnchorName("apsdofjaspıdjp");
                fmt.setFontFamily(monospaceFontFamily);
                fmt.setBackground(highlightColor);
            }
            if (highlightItem.isUnderLine)
                fmt.setUnderlineStyle(QTextCharFormat::SingleUnderline);
            else
                fmt.setUnderlineStyle(QTextCharFormat::NoUnderline);

            setFormat(match.capturedStart(), match.capturedLength(), fmt);
        }
        //}
    }
}

void SyntaxHighlighter::initRegexVector() {
    // Quotes
    regexVector.append(regexVectorItem("\"(.*?)\"", QColor::fromRgb(152, 166, 74), Qt::white));

    // Blockquote
    // regexVector.append(regexVectorItem("^[[:space:]]*>.*$", QColor::fromRgb(68,131,93),
    // Qt::white));
    regexVector.append(regexVectorItem("^\\s*>.*$", QColor::fromRgb(68, 131, 93), Qt::white));

    // Links
    regexVector.append(regexVectorItem(
        "(http[s]?:\\/\\/(www\\.)?|ftp:\\/\\/"
        "(www\\.)?|www\\.){1}([0-9A-Za-z-\\.@:%_\+~#=]+)+((\\.[a-zA-Z]{2,3})+)(/(.)*)?(\\?(.)*)?",
        Qt::blue, Qt::white, false, false, true));

    // Italic
    // regexVector.append(regexVectorItem("\\*[A-z0-9]+\\*", Qt::black, Qt::white, false, true));
    regexVector.append(regexVectorItem("\\*{1}[^*.]+\\*{1}", Qt::black, Qt::white, false, true));
    regexVector.append(regexVectorItem("\\_{1}[^_.]+\\_{1}", Qt::black, Qt::white, false, true));
    // Bold
    // regexVector.append(regexVectorItem("\\*\\*[A-z0-9]+\\*\\*", Qt::black, Qt::white, true));
    regexVector.append(regexVectorItem("\\*{2}[^*.]+\\*{2}", Qt::black, Qt::white, true));
    regexVector.append(regexVectorItem("\\_{2}[^_.]+\\_{2}", Qt::black, Qt::white, true));
    // BoldItalic
    // regexVector.append(regexVectorItem("\\*\\*\\*[A-z0-9]+\\*\\*\\*", Qt::black, Qt::white, true,
    // true));
    regexVector.append(regexVectorItem("\\*{3}[^*.]+\\*{3}", Qt::black, Qt::white, true, true));
    regexVector.append(regexVectorItem("\\_{3}[^_.]+\\_{3}", Qt::black, Qt::white, true, true));

    // Strikethrough
    HighlightItem strikeThrough = regexVectorItem("\\~[A-z0-9]+\\~", Qt::black, Qt::white);
    strikeThrough.isStrikeThrough = true;
    regexVector.append(strikeThrough);

    // Monospace
    regexVector.append(regexVectorItem("`.+`", Qt::black, Qt::white, false, false, false, true));

    regexVector.append(regexVectorItem("✔((?!✖).)*", Qt::darkGreen, Qt::white));
    regexVector.append(regexVectorItem("✖((?!✔).)*", Qt::darkRed, Qt::white));
    // Lambda is broken, let it be
    regexVector.append(regexVectorItem("λ.*", QColor(8, 129, 199), Qt::white));
    // Headings through 1 to 6
    for (int i = 1; i < 7; i++) {
        HighlightItem heading =
            regexVectorItem(QString("(?m)^#{%1} (?!#)(.*)").arg(QString::number(i)), qRgb(43, 82, 130),
                            Qt::white, false);
        regexVector.append(heading);
    }
}

HighlightItem SyntaxHighlighter::regexVectorItem(QString exp, QColor fore, QColor back, bool isBold,
                                                 bool isItalic, bool isUnderLine,
                                                 bool isMonospace) {
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
