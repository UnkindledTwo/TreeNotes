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
    TreeWidgetItem* parent() const;
    TreeWidgetItem* takeChild(int index);

    TreeWidgetItem();

    QString message = "";

    int iconVectorIndex = 6;

    QDateTime lastEdited = QDateTime::currentDateTime();

    TreeWidgetItem *clone();

    bool starred = false;

    bool readOnly = false;

    QString toString();
};

#endif // TREEWIDGETITEM_H
