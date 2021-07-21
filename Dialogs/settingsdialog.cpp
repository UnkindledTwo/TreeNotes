#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    this->adjustSize();
    this->setFixedSize(this->size());
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::setAppConfig(app_config appConfig){
    this->m_appConfig = appConfig;
    ui->notetree_select_new_items->setChecked(appConfig.notetree_select_new_items);
    ui->notetree_alternating_row_colors->setChecked(appConfig.notetree_alternating_row_colors);
    ui->notetree_indentation_size->setValue(appConfig.notetree_indentation_size);
    ui->opacity->setValue(appConfig.opacity);
    ui->notetree_animated->setChecked(appConfig.notetree_animated);
    ui->double_click_to_edit->setChecked(appConfig.doubleClickToEditMessage);
    ui->layout_margin->setValue(appConfig.layoutMargin);
    ui->splitter_handle_width->setValue(appConfig.splitter_handle_width);
    ui->confirm_delete->setChecked(appConfig.confirm_delete);
    ui->line_wrapping->setChecked(appConfig.line_wrapping);
    ui->pair_completion->setChecked(appConfig.pair_completion);
    ui->notetree_drag_drop->setChecked(appConfig.notetree_drag_drop);
    ui->maximum_backups->setValue(appConfig.maximum_backups);
    ui->highlight_current_line->setChecked(appConfig.highlight_current_line);
    ui->use_native_theme->setChecked(appConfig.use_native_theme);
}

struct app_config SettingsDialog::appConfig(){
    m_appConfig.notetree_select_new_items = ui->notetree_select_new_items->isChecked();
    m_appConfig.notetree_alternating_row_colors = ui->notetree_alternating_row_colors->isChecked();
    m_appConfig.notetree_indentation_size = ui->notetree_indentation_size->value();
    m_appConfig.opacity = ui->opacity->value();
    m_appConfig.notetree_animated = ui->notetree_animated->isChecked();
    m_appConfig.doubleClickToEditMessage = ui->double_click_to_edit->isChecked();
    m_appConfig.layoutMargin = ui->layout_margin->value();
    m_appConfig.splitter_handle_width = ui->splitter_handle_width->value();
    m_appConfig.confirm_delete = ui->confirm_delete->isChecked();
    m_appConfig.line_wrapping = ui->line_wrapping->isChecked();
    m_appConfig.pair_completion = ui->pair_completion->isChecked();
    m_appConfig.notetree_drag_drop = ui->notetree_drag_drop->isChecked();
    m_appConfig.maximum_backups = ui->maximum_backups->value();
    m_appConfig.highlight_current_line = ui->highlight_current_line->isChecked();
    m_appConfig.use_native_theme = ui->use_native_theme->isChecked();

    return m_appConfig;
}
