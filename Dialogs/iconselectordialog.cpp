#include "iconselectordialog.h"
#include "ui_iconselectordialog.h"

IconSelectorDialog::IconSelectorDialog(QWidget *parent, QVector<QIcon> iconVector, int initialPosition) :
    QDialog(parent),
    ui(new Ui::IconSelectorDialog)
{
    ui->setupUi(this);

    for(int i = 0; i < iconVector.count(); i++){
        icons.append(iconVector[i]);
    }

    for(int i = 0; i < icons.count(); i++){
        ui->iconBox->addItem(icons[i], "");
    }

    ui->iconBox->setCurrentIndex(initialPosition);
    ui->iconBox->setFixedSize(QSize(60,36));
}

IconSelectorDialog::~IconSelectorDialog()
{
    delete ui;
}

void IconSelectorDialog::on_iconBox_currentIndexChanged(const QString &arg1)
{
    QTreeWidgetItemIterator it(ui->treeWidget);
    while(*it){
        (*it)->setIcon(0, icons[ui->iconBox->currentIndex()]);
        ++it;
    }
    selectedIcon = icons[ui->iconBox->currentIndex()];
    index = ui->iconBox->currentIndex();
}

void IconSelectorDialog::copyFrom(TreeWidget *w){
    auto mo = w->metaObject();
    do{
        for(int i = mo->propertyOffset(); i < mo->propertyCount(); i++){
            ui->treeWidget->setProperty(mo->property(i).name(), mo->property(i).read(w));
        }
    }
    while ((mo = mo->superClass()));

    //TreeWidget gets hidden when the tree widget in the main window is hidden.
    ui->treeWidget->setHidden(false);

    ui->treeWidget->resizeColumnToContents(1);
    ui->treeWidget->expandAll();
    ui->treeWidget->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->treeWidget->resizeColumnToContents(1);

}

