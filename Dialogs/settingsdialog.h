#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QColorDialog>
#include <QDebug>
#include <QDialog>

#include "globals.h"
#include "../appconfig.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

    void setAppConfig(struct app_config appConfig);
    struct app_config appConfig();

private slots:

    void on_bResetSettings_clicked();

private:
    Ui::SettingsDialog *ui;
    struct app_config m_appConfig;
};

#endif  // SETTINGSDIALOG_H
