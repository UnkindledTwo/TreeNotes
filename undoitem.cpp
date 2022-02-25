#include "undoitem.h"

UndoItem::UndoItem(QTreeWidgetItem *item, QTreeWidgetItem *parent, bool isTopLevelItem,
                   unsigned position) {
    this->item = item;
    this->parent = parent;
    this->isTopLevelItem = isTopLevelItem;
    this->position = position;
}
