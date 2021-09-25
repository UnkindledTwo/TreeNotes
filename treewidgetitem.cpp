#include "treewidgetitem.h"


TreeWidgetItem::TreeWidgetItem()
{

}

TreeWidgetItem *TreeWidgetItem::clone()
{
    TreeWidgetItem *ret = new TreeWidgetItem();
    ret->setIcon(0, this->icon(0));
    ret->setIcon(1, this->icon(1));
    ret->setText(0, this->text(0));
    ret->starred = this->starred;
    ret->message = this->message;
    ret->lastEdited = this->lastEdited;
    ret->iconVectorIndex = this->iconVectorIndex;
    ret->readOnly = this->readOnly;

    QList<QTreeWidgetItem*> items;

    for(int i = 0; i < this->childCount(); i++){
        items.append(((TreeWidgetItem*)this->child(i))->clone());
    }

    ret->addChildren(items);
    qDebug() << ret->toString();
    return ret;
}

QString TreeWidgetItem::toString()
{
    QString res = QString("TreeWidgetItem: {Title: %1, Message: %2, Starred: %3, Last Edited: %4, Icon Index: %5, Read Only: %6}")
            .arg(this->text(0))
            .arg(this->message)
            .arg(QString::number(this->starred))
            .arg(this->lastEdited.toString())
            .arg(QString::number(this->iconVectorIndex))
            .arg(QString::number(this->readOnly))
            ;
    return res;
}

TreeWidgetItem *TreeWidgetItem::parent() const
{
    return (TreeWidgetItem*)(QTreeWidgetItem::parent());
}

TreeWidgetItem *TreeWidgetItem::takeChild(int index)
{
    return (TreeWidgetItem*)(QTreeWidgetItem::takeChild(index));
}
