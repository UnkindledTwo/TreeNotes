#ifndef UNDOITEM_H
#define UNDOITEM_H

#include <QTreeWidget>
#include <QDebug>

class UndoItem
{
public:
    UndoItem(QTreeWidgetItem *item = 0, QTreeWidgetItem *parent = 0, bool isTopLevelItem = true);

    QTreeWidgetItem* item;
    QTreeWidgetItem* parent;
    bool isTopLevelItem;

};

#endif // UNDOITEM_H
