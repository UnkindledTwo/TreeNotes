#ifndef ICONSELECTORDIALOG_H
#define ICONSELECTORDIALOG_H

#include <QDebug>
#include <QDialog>
#include <QMetaObject>
#include <QMetaProperty>
#include <QStyle>
#include <QTreeWidget>
#include <QTreeWidgetItemIterator>
#include <QVector>

#include "treewidget.h"

namespace Ui {
class IconSelectorDialog;
}

class IconSelectorDialog : public QDialog {
    Q_OBJECT

public:
    void copyFrom(TreeWidget *w);
    explicit IconSelectorDialog(QWidget *parent = nullptr,
                                QVector<QIcon> iconVector = QVector<QIcon>(),
                                int initialPosition = 0);
    ~IconSelectorDialog();

    QIcon selectedIcon;

    unsigned short int index = 0;

private slots:
    void on_iconBox_currentIndexChanged(const QString &arg1);

private:
    Ui::IconSelectorDialog *ui;

    QVector<QIcon> icons;
};

#endif  // ICONSELECTORDIALOG_H
