#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QWidget>
#include <QTreeWidget>

namespace Ui {
class TreeWidget;
}

class TreeWidget : public QTreeWidgetItem
{
    Q_OBJECT

public:
    explicit TreeWidget(QWidget *parent = nullptr);
    ~TreeWidget();

private:
    Ui::TreeWidget *ui;
};

#endif // TREEWIDGET_H
