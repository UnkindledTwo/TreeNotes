#include "treewidget.h"
#include "ui_treewidget.h"

TreeWidget::TreeWidget(QWidget *parent) :
    QTreeWidget(parent),
    ui(new Ui::TreeWidget)
{
    ui->setupUi(this);
}

TreeWidget::~TreeWidget()
{
    delete ui;
}

void TreeWidget::addAllChildren(QTreeWidgetItem *item ,QVector<QTreeWidgetItem*>* items){
    for(int i = 0; i < item->childCount(); i++){
        items->append(item->child(i));
        addAllChildren(item->child(i), items);
    }
}

void TreeWidget::expandChildren(TreeWidgetItem *i){
    if(!i){
        this->expandAll();
        return;
    }

    QVector<QTreeWidgetItem*> items;
    i->setExpanded(true);
    this->addAllChildren(i, &items);
    for(int i = 0; i < items.count(); i++){
        items[i]->setExpanded(true);
    }
}

void TreeWidget::collapseChildren(TreeWidgetItem *i){
    if(!i){
        this->collapseAll();
        return;
    }

    QVector<QTreeWidgetItem*> items;
    i->setExpanded(false);
    this->addAllChildren(i, &items);
    for(int i = 0; i < items.count(); i++){
        items[i]->setExpanded(false);
    }
}

void TreeWidget::setDragDrop(bool a){
    this->setDragEnabled(a);
    this->viewport()->setAcceptDrops(a);
    this->setDropIndicatorShown(a);
    if(a)
        this->setDragDropMode(QAbstractItemView::InternalMove);
    else
        this->setDragDropMode(QAbstractItemView::NoDragDrop);
}

int TreeWidget::noteCount(){
    int cnt = 0;
    QTreeWidgetItemIterator it(this);
    while(*it){
        cnt++;
        *it++;
    }
    return cnt;
}
