#include "undoitem.h"

UndoItem::UndoItem(QTreeWidgetItem *item, QTreeWidgetItem *parent, bool isTopLevelItem)
{
    this->item = item;
    this->parent = parent;
    this->isTopLevelItem = isTopLevelItem;
}
