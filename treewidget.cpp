#include "treewidget.h"
#include "ui_treewidget.h"

TreeWidget::TreeWidget(QWidget *parent) :
    QTreeWidgetItem(),
    ui(new Ui::TreeWidget)
{
    ui->setupUi((QWidget*)this);
}

TreeWidget::~TreeWidget()
{
    delete ui;
}
