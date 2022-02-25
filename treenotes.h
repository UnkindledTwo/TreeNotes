#ifndef TREENOTES_H
#define TREENOTES_H

#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QFontDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QKeyEvent>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPrinter>
#include <QPushButton>
#include <QScrollBar>
#include <QSettings>
#include <QShortcut>
#include <QSplitter>
#include <QStyle>
#include <QStyleFactory>
#include <QTimer>
#include <QVBoxLayout>
#include <QtConcurrent>

// Win only
//#include <QtWinExtras>
#ifdef Q_OS_WIN
#include <QtWin>
#endif

#include "Dialogs/iconselectordialog.h"
#include "Dialogs/macrohelp.h"
#include "Dialogs/searchinallnotesdialog.h"
#include "Dialogs/settingsdialog.h"
#include "Dialogs/tageditordialog.h"
#include "Dialogs/taginfodialog.h"
#include "Dialogs/tagresultsdialog.h"
#include "Dialogs/newnotedialog.h"
#include "appconfig.h"
#include "plaintextedit.h"
#include "saver.h"
#include "scratchpad.h"
#include "treewidget.h"
#include "treewidgetitem.h"
#include "undoitem.h"
#include "globals.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class TreeNotes;
}
QT_END_NAMESPACE

class TreeNotes : public QMainWindow
{
    Q_OBJECT

public:
    QString saveFileName = "save.xml";

    TreeNotes(QWidget *parent = nullptr, QString saveFileName = "save.xml");
    ~TreeNotes();

private slots:
    void showNoSelectedItemDialog();
    QString latestVersion();
    void updateWindowTitleAndIcon();
    bool hasUnsavedChanges();
    QString dateTimeNoSpace();
    void AttemptSaveBackup();
    void Save(TreeWidgetItem *target);
    void Delete(QTreeWidgetItem *target);
    void ReadAppConfig(struct app_config appConfig);
    void ReadQSettings();
    void RefreshLabels();
    void InitStatusLabels();
    void InitShortcuts();
    void InitMacroVector();
    void SaveQSettings();
    void InitIconVector();
    void SaveToFile();
    void ReadFromFile();
    void MoveUp(TreeWidgetItem *item);
    void MoveDown(TreeWidgetItem *item);
    void ShowContextMenu(const QPoint &pos);
    void CleanBackups(int max, QString backupsDir);
    void ApplyMacroVector();
    void openScratchPad(int index);
    void createScratchPad(QString name);

    QString boolToString(bool a);
    bool stringToBool(QString a);

    // UI events
    void on_scratchpad_action_triggered();
    void on_tag_doubleClicked(QListWidgetItem *item);
    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void on_actionAdd_triggered();
    void on_actionSave_triggered();
    void on_actionDelete_triggered();
    void on_actionMove_Up_triggered();
    void on_actionMove_Down_triggered();
    void on_actionSet_Icon_triggered();
    void on_actionFocus_Parent_triggered();
    void on_actionHide_Show_Note_Tree_triggered();
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_actionQt_triggered();
    void on_actionSet_Font_Text_Editors_triggered();
    void on_actionImport_Text_File_triggered();
    void on_actionExport_Text_File_triggered();
    void on_messageEdit_textChanged();
    void on_actionMacros_triggered();
    void on_actionUndo_Delete_triggered();
    void on_actionSettings_triggered();
    void on_actionStar_Unstar_triggered();
    void on_actionExpand_All_triggered();
    void on_actionCollapse_All_triggered();
    void on_actionRead_Only_toggled(bool arg1);
    void on_titleEdit_textChanged(const QString &arg1);
    void on_actionExport_PDF_triggered();
    void on_actionSearch_In_Current_Note_triggered();
    void on_actionReplace_All_In_Current_Note_triggered();
    void on_actionSearch_In_All_Notes_triggered();
    void on_actionHide_Show_Toolbar_triggered();
    void on_actionFullscreen_On_Off_triggered();
    void on_actionHide_Show_Title_2_triggered();
    void on_actionClone_triggered();
    void on_actionChange_Layout_triggered();
    void on_actionCheck_For_The_Latest_Version_triggered();
    void on_actionChange_Tag_triggered();
    void on_actionHide_Show_Tags_triggered();
    void on_actionTag_Info_triggered();
    void on_actionNew_Scratch_Pad_triggered();

    void on_actionHighlighting_triggered(bool checked);

private:
    Ui::TreeNotes *ui;

    TreeWidget *noteTree;
    QSplitter *splitter;
    QListWidget *tagsList;

    QLabel *noteCntLabel;
    QLabel *childrenCntLabel;
    QLabel *dateTimeLabel;
    QLabel *lineCountLabel;
    QLabel *lengthLabel;
    QLabel *currentPositionLabel;
    QLabel *readOnlyLabel;

    bool doubleClickToEditMessage;
    QVector<QPair<QString, std::function<QString()>>> macroVec;

    QVector<UndoItem> undoVector;
    QVector<ScratchPad *> scratchPads;

protected:
    void closeEvent(QCloseEvent *e);
    void keyPressEvent(QKeyEvent *e);
};

#define SHORTCUT_JUMP_MSG "Ctrl+H"
#define SHORTCUT_JUMP_TITLE "Ctrl+J"
#define SHORTCUT_JUMP_TREE "Ctrl+K"

#define WINDOW_TITLE_DEFAULT "TreeNotes"

#endif  // TREENOTES_H
