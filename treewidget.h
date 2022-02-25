#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QKeyEvent>
#include <QTreeWidget>
#include <QWidget>

#include "treewidgetitem.h"
#include "globals.h"

namespace Ui {
class TreeWidget;
}

class TreeWidget : public QTreeWidget {
    Q_OBJECT

public:
    TreeWidgetItem *currentItem();
    TreeWidgetItem *takeTopLevelItem(int index);

    void expandChildren(TreeWidgetItem *i);
    void collapseChildren(TreeWidgetItem *i);
    void setDragDrop(bool);
    void addAllChildren(QTreeWidgetItem *item, QVector<QTreeWidgetItem *> *items);
    void setStar(TreeWidgetItem *, bool);
    int noteCount();
    TreeWidgetItem *addNote(TreeWidgetItem *parent, QString text, QString message = "", int iconVectorIndex = 0);
    TreeWidgetItem *addNote(TreeWidgetItem *parent, TreeWidgetItem *item);
    TreeWidgetItem *createNote(QString text, QString message = "", int iconVectorIndex = 0);

    explicit TreeWidget(QWidget *parent = nullptr);
    ~TreeWidget();

protected:
    void mousePressEvent(QMouseEvent *e);

signals:
    void middleClicked();

private:
    Ui::TreeWidget *ui;
};

#endif  // TREEWIDGET_H
