#ifndef ICONSELECTORDIALOG_H
#define ICONSELECTORDIALOG_H

#include <QDialog>
#include <QVector>
#include <QStyle>
#include <QTreeWidget>

namespace Ui {
class IconSelectorDialog;
}

class IconSelectorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IconSelectorDialog(QWidget *parent = nullptr, QVector<QIcon> iconVector = QVector<QIcon>());
    ~IconSelectorDialog();

    QIcon selectedIcon;

    unsigned short int index = 0;

private slots:
    void on_iconBox_currentIndexChanged(const QString &arg1);

private:
    Ui::IconSelectorDialog *ui;

    QVector<QIcon> icons;

    QTreeWidgetItem *i1;
    QTreeWidgetItem *i2;
    QTreeWidgetItem *i3;
};

#endif // ICONSELECTORDIALOG_H
