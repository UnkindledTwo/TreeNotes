#ifndef HIGHLIGHTITEM_H
#define HIGHLIGHTITEM_H

#include <QColor>
#include <QFont>
#include <QString>

enum class HighlightFontSize
{
    Same,
    Double,
    Half
};

enum class HighlightFontFamily
{
    Regular,
    Monospace
};

class HighlightItem
{
public:
    // HighlightItem()

    bool isBold = false;
    bool isItalic = false;
    bool isUnderLine = false;
    bool isStrikeThrough = false;
    bool isMonospace = false;
    QString regex;
    QColor background;
    QColor foreground;
    HighlightFontSize fontSize;
};

#endif  // HIGHLIGHTITEM_H
