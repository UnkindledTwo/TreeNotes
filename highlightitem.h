#ifndef HIGHLIGHTITEM_H
#define HIGHLIGHTITEM_H

#include <QString>
#include <QColor>
#include <QFont>

enum class HighlightFontSize{
    Twice,
    Half,
    Same
};

enum class HighlightFontFamily{
    Regular,
    Monospace
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
    HighlightFontFamily highlightFontFamily = HighlightFontFamily::Regular;
};

#endif // HIGHLIGHTITEM_H
