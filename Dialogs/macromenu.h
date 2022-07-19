#pragma once

#include <QDialog>
#include <QListWidget>
#include <QPair>
#include <QKeyEvent>

#include "globals.h"

namespace Ui {
class MacroMenu;
}

class MacroMenu : public QDialog
{
    Q_OBJECT

public:
    explicit MacroMenu(QWidget *parent = nullptr);
    ~MacroMenu();

protected:
    void keyPressEvent(QKeyEvent *e);

private slots:
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

signals:
    void itemSelected(int index);

private:
    Ui::MacroMenu *ui;
};

