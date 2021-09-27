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
    TreeWidgetItem *clone();

    QString message = "";
    int iconVectorIndex = 6;
    QDateTime lastEdited = QDateTime::currentDateTime();
    bool starred = false;
    bool readOnly = false;
    QString toString();
    QStringList tags;
};

#endif // TREEWIDGETITEM_H
