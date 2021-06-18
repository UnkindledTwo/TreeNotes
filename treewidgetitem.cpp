#include "treewidgetitem.h"

TreeWidgetItem::TreeWidgetItem()
{

}

TreeWidgetItem* TreeWidgetItem::_clone(){
    TreeWidgetItem *clone = (TreeWidgetItem*)this->clone();
    clone->message = this->message;
    clone->setText(0, this->text(0));

    return clone;
}
