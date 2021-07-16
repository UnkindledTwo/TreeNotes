#ifndef TREEWIDGETITEM_H
#define TREEWIDGETITEM_H

#include <QObject>
#include <QWidget>
#include <QTreeWidget>
#include <QDateTime>

class TreeWidgetItem : public QTreeWidgetItem
{
public:
    TreeWidgetItem();

    QString message = "";

    int iconVectorIndex = 6;

    QDateTime lastEdited = QDateTime::currentDateTime();

    TreeWidgetItem *_clone();

    bool starred = false;

    int hScrollbarPos;
    int vScrollbarPos;

    bool readOnly = false;
};

#endif // TREEWIDGETITEM_H
