#ifndef TREEWIDGETITEM_H
#define TREEWIDGETITEM_H

#include <QObject>
#include <QWidget>
#include <QTreeWidget>

class TreeWidgetItem : public QTreeWidgetItem
{
public:
    TreeWidgetItem();

    QString message = "";

    int iconVectorIndex = 6;
};

#endif // TREEWIDGETITEM_H
