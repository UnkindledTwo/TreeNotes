#ifndef HIGHLIGHTITEM_H
#define HIGHLIGHTITEM_H

#include <QString>
#include <QColor>
#include <QFont>

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
    HighlightFontFamily highlightFontFamily = HighlightFontFamily::Regular;
};

#endif // HIGHLIGHTITEM_H
