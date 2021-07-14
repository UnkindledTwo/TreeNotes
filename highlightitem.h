#ifndef HIGHLIGHTITEM_H
#define HIGHLIGHTITEM_H

#include <QString>
#include <QColor>
#include <QFont>

enum class HighlightFontSize{
    Same,
    Double,
    Half
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
    bool isStrikeThrough = false;
    QString regex;
    QColor background;
    QColor foreground;
    HighlightFontFamily highlightFontFamily = HighlightFontFamily::Regular;
    HighlightFontSize highlightFontSize = HighlightFontSize::Same;
};

#endif // HIGHLIGHTITEM_H
