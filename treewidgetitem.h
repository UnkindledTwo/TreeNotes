#ifndef TREEWIDGETITEM_H
#define TREEWIDGETITEM_H

#include <QDateTime>
#include <QDebug>
#include <QObject>
#include <QTreeWidget>
#include <QWidget>

class TreeWidgetItem : public QTreeWidgetItem {
public:
    TreeWidgetItem* parent() const;
    TreeWidgetItem* takeChild(int index);
    TreeWidgetItem();
    TreeWidgetItem* clone();

    QString message = "";
    int iconVectorIndex = 6;
    QDateTime lastEdited = QDateTime::currentDateTime();
    bool starred = false;
    bool readOnly = false;
    QString toString();
    QStringList tags;
    bool highlighting = true;
};

#endif  // TREEWIDGETITEM_H
