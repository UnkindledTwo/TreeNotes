#ifndef SCRATCHPAD_H
#define SCRATCHPAD_H

#include <QCloseEvent>
#include <QLabel>
#include <QMainWindow>

#include "plaintextedit.h"

namespace Ui {
class ScratchPad;
}

class ScratchPad : public QMainWindow {
    Q_OBJECT

signals:
    void closed();
    void deleteRequested(ScratchPad *pad);

public:
    explicit ScratchPad(QWidget *parent = nullptr);
    ~ScratchPad();

    PlainTextEdit *textEdit;
private slots:
    void createStatusLabels();
    void updateStatusLabels();
    void on_actionDelete_Scratch_Pad_triggered();

    void on_actionRead_Only_triggered(bool checked);

private:
    Ui::ScratchPad *ui;

    QLabel *l_length;
    QLabel *l_col;
    QLabel *l_row;
    QLabel *l_readOnly;

protected:
    void closeEvent(QCloseEvent *e);
};

#endif  // SCRATCHPAD_H
