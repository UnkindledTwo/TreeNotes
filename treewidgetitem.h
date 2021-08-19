#ifndef TREEWIDGETITEM_H
#define TREEWIDGETITEM_H

#include <QObject>
#include <QWidget>
#include <QTreeWidget>
#include <QDateTime>
#include <QDebug>

class TreeWidgetItem : public QTreeWidgetItem
{
public:
    TreeWidgetItem();

    QString message = "";

    int iconVectorIndex = 6;

    QDateTime lastEdited = QDateTime::currentDateTime();

    TreeWidgetItem *clone();

    bool starred = false;

    int hScrollbarPos;
    int vScrollbarPos;
    int cursorpos = 0;

    bool readOnly = false;

    QString toString();
};

#endif // TREEWIDGETITEM_H
