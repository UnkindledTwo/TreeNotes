#include "iconselectordialog.h"
#include "ui_iconselectordialog.h"

IconSelectorDialog::IconSelectorDialog(QWidget *parent, QVector<QIcon> iconVector) :
    QDialog(parent),
    ui(new Ui::IconSelectorDialog)
{
    ui->setupUi(this);

    i1 = new QTreeWidgetItem();
    i1->setText(0, "Parent1");
    i1->setExpanded(true);
    i2 = new QTreeWidgetItem();
    i2->setText(0, "Child1");
    i3 = new QTreeWidgetItem();
    i3->setText(0, "Parent2");
    i1->addChild(i2);
    ui->treeWidget->addTopLevelItem(i1);
    ui->treeWidget->addTopLevelItem(i3);

    for(int i = 0; i < iconVector.count(); i++){
        icons.append(iconVector[i]);
    }

    for(int i = 0; i < icons.count(); i++){
        ui->iconBox->addItem(icons[i], "");
    }
}

IconSelectorDialog::~IconSelectorDialog()
{
    delete ui;
}

void IconSelectorDialog::on_iconBox_currentIndexChanged(const QString &arg1)
{
    selectedIcon = icons[ui->iconBox->currentIndex()];
    i1->setIcon(0, icons[ui->iconBox->currentIndex()]);
    i2->setIcon(0, icons[ui->iconBox->currentIndex()]);
    i3->setIcon(0, icons[ui->iconBox->currentIndex()]);

    index = ui->iconBox->currentIndex();
}

