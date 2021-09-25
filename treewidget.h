#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QKeyEvent>

#include "treewidgetitem.h"

namespace Ui {
class TreeWidget;
}

class TreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    TreeWidgetItem *currentItem();
    TreeWidgetItem *takeTopLevelItem(int index);

    void expandChildren(TreeWidgetItem *i);
    void collapseChildren(TreeWidgetItem *i);
    void setDragDrop(bool);
    void addAllChildren(QTreeWidgetItem *item ,QVector<QTreeWidgetItem*>* items);
    void setStar(TreeWidgetItem*, bool);
    int noteCount();
    explicit TreeWidget(QWidget *parent = nullptr);
    ~TreeWidget();

private:
    Ui::TreeWidget *ui;
};

#endif // TREEWIDGET_H
