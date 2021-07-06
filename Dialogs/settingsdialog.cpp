#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::setAppConfig(app_config appConfig){
    this->m_appConfig = appConfig;
    ui->notetree_alternating_row_colors->setChecked(appConfig.notetree_alternating_row_colors);
    ui->notetree_select_rows->setChecked(appConfig.notetree_select_rows);
    ui->notetree_indentation_size->setValue(appConfig.notetree_indentation_size);
    ui->opacity->setValue(appConfig.opacity);
    ui->notetree_animated->setChecked(appConfig.notetree_animated);
    ui->double_click_to_edit->setChecked(appConfig.doubleClickToEditMessage);
    ui->layout_margin->setValue(appConfig.layoutMargin);
    ui->splitter_handle_width->setValue(appConfig.splitter_handle_width);
    ui->confirm_delete->setChecked(appConfig.confirm_delete);
    ui->line_wrapping->setChecked(appConfig.line_wrapping);
}

struct app_config SettingsDialog::appConfig(){
    m_appConfig.notetree_alternating_row_colors = ui->notetree_alternating_row_colors->isChecked();
    m_appConfig.notetree_select_rows = ui->notetree_select_rows->isChecked();
    m_appConfig.notetree_indentation_size = ui->notetree_indentation_size->value();
    m_appConfig.opacity = ui->opacity->value();
    m_appConfig.notetree_animated = ui->notetree_animated->isChecked();
    m_appConfig.doubleClickToEditMessage = ui->double_click_to_edit->isChecked();
    m_appConfig.layoutMargin = ui->layout_margin->value();
    m_appConfig.splitter_handle_width = ui->splitter_handle_width->value();
    m_appConfig.confirm_delete = ui->confirm_delete->isChecked();
    m_appConfig.line_wrapping = ui->line_wrapping->isChecked();

    return m_appConfig;
}
