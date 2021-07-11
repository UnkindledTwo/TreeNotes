#ifndef HIGHLIGHTITEM_H
#define HIGHLIGHTITEM_H

#include <QString>
#include <QColor>

enum class HighlightFontSize{
    Twice,
    Half,
    Same
};

class HighlightItem
{
public:
    HighlightItem();

    bool isBold = false;
    bool isItalic = false;
    bool isUnderLine = false;
    QString regex;
    QColor background;
    QColor foreground;
    HighlightFontSize highlighFontSize;
};

#endif // HIGHLIGHTITEM_H
