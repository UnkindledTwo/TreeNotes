#include "treewidget.h"

#include "ui_treewidget.h"

TreeWidget::TreeWidget(QWidget *parent) : QTreeWidget(parent), ui(new Ui::TreeWidget) {
    ui->setupUi(this);
}

TreeWidget::~TreeWidget() { delete ui; }

void TreeWidget::mousePressEvent(QMouseEvent *e) {
    QTreeWidget::mousePressEvent(e);
    if (e->button() == Qt::MiddleButton) {
        e->ignore();
        emit middleClicked();
    }
}

void TreeWidget::addAllChildren(QTreeWidgetItem *item, QVector<QTreeWidgetItem *> *items) {
    for (int i = 0; i < item->childCount(); i++) {
        items->append(item->child(i));
        addAllChildren(item->child(i), items);
    }
}

TreeWidgetItem *TreeWidget::currentItem() { return (TreeWidgetItem *)(QTreeWidget::currentItem()); }

TreeWidgetItem *TreeWidget::takeTopLevelItem(int index) {
    return (TreeWidgetItem *)(QTreeWidget::takeTopLevelItem(index));
}

void TreeWidget::expandChildren(TreeWidgetItem *i) {
    if (!i) {
        this->expandAll();
        return;
    }

    QVector<QTreeWidgetItem *> items;
    i->setExpanded(true);
    this->addAllChildren(i, &items);
    for (int i = 0; i < items.count(); i++) {
        items[i]->setExpanded(true);
    }
}

void TreeWidget::collapseChildren(TreeWidgetItem *i) {
    if (!i) {
        this->collapseAll();
        return;
    }

    QVector<QTreeWidgetItem *> items;
    i->setExpanded(false);
    this->addAllChildren(i, &items);
    for (int i = 0; i < items.count(); i++) {
        items[i]->setExpanded(false);
    }
}

void TreeWidget::setDragDrop(bool a) {
    this->setDragEnabled(a);
    this->viewport()->setAcceptDrops(a);
    this->setDropIndicatorShown(a);
    if (a)
        this->setDragDropMode(QAbstractItemView::InternalMove);
    else
        this->setDragDropMode(QAbstractItemView::NoDragDrop);
}

int TreeWidget::noteCount() {
    int cnt = 0;
    QTreeWidgetItemIterator it(this);
    while (*it) {
        cnt++;
        *it++;
    }
    return cnt;
}

void TreeWidget::setStar(TreeWidgetItem *i, bool s) {
    i->setStarred(s);
    /*
    if (s) {
        // i->setText(1, "★");
        i->setIcon(1, QIcon(":/Resources/Icons/star.png"));
    } else {
        // i->setText(1, "");
        i->setIcon(1, QIcon());
    }

    i->starred = s;
*/
    this->resizeColumnToContents(1);
}

TreeWidgetItem *TreeWidget::addNote(TreeWidgetItem *parent, QString text, QString message, int iconVectorIndex)
{
    TreeWidgetItem *itemToAdd = createNote(text, message, iconVectorIndex);

    if (parent == NULL) {
        // New Item has no parent
        addTopLevelItem(itemToAdd);
        goto topLevel;
    }

    parent->addChild(itemToAdd);
    parent->setExpanded(true);

topLevel:
    if (Globals::appConfig.notetree_select_new_items) this->setCurrentItem(itemToAdd);

    return itemToAdd;
}

TreeWidgetItem *TreeWidget::addNote(TreeWidgetItem *parent, TreeWidgetItem *item)
{
    if (parent == NULL) {
        // New Item has no parent
        addTopLevelItem(item);
        goto topLevel;
    }

    parent->addChild(item);
    parent->setExpanded(true);

topLevel:
    if (Globals::appConfig.notetree_select_new_items) this->setCurrentItem(item);

    return item;
}

TreeWidgetItem *TreeWidget::createNote(QString text, QString message, int iconVectorIndex)
{
    //Construct new note
    TreeWidgetItem *newItem = new TreeWidgetItem();
    newItem->setIcon(0, Globals::iconVector[iconVectorIndex]);
    newItem->setText(0, text);
    newItem->iconVectorIndex = iconVectorIndex;
    newItem->message = message;
    return newItem;
}
