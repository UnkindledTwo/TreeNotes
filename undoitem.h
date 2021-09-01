#ifndef UNDOITEM_H
#define UNDOITEM_H

#include <QTreeWidget>
#include <QDebug>

class UndoItem
{
public:
    UndoItem(QTreeWidgetItem *item = 0, QTreeWidgetItem *parent= 0, bool isTopLevelItem = true, unsigned position = 0);

    QTreeWidgetItem* item;
    QTreeWidgetItem* parent;
    bool isTopLevelItem;
    unsigned position;

};

#endif // UNDOITEM_H
