#ifndef MACROHELP_H
#define MACROHELP_H

#include <QDialog>
#include <QListWidget>
#include <QPlainTextEdit>
#include <QSplitter>
#include <QTextEdit>
#include <QVector>
#include <functional>

#include "globals.h"

namespace Ui {
class MacroHelp;
}

class MacroHelp : public QDialog {
    Q_OBJECT

public:
    QPlainTextEdit* plainTextEdit();
    QListWidget* listWidget();

    explicit MacroHelp(QWidget* parent = nullptr);
    ~MacroHelp();

private:
    Ui::MacroHelp* ui;
};

#endif  // MACROHELP_H
