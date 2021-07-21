#ifndef TREENOTES_H
#define TREENOTES_H

#include <QMainWindow>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QStyle>
#include <QShortcut>
#include <QMessageBox>
#include <QKeyEvent>
#include <QTextStream>
#include <QLabel>
#include <QTimer>
#include <QSplitter>
#include <QFontDialog>
#include <QSettings>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QScrollBar>
#include <QtConcurrent>
#include <QPushButton>
#include <QPrinter>

//#include <QtXml>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>

//Win only
//#include <QtWinExtras>
#ifdef Q_OS_WIN
#include <QtWin>
#endif

#include "Dialogs/settingsdialog.h"
#include "Dialogs/iconselectordialog.h"
#include "Dialogs/macrohelp.h"

#include "plaintextedit.h"
#include "treewidgetitem.h"
#include "undoitem.h"
#include "appconfig.h"
#include "treewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TreeNotes; }
QT_END_NAMESPACE

class TreeNotes : public QMainWindow
{
    Q_OBJECT

public:
    QVector <QIcon> iconVector;

    void closeEvent(QCloseEvent *e);
    void keyPressEvent(QKeyEvent *e);

    TreeNotes(QWidget *parent = nullptr);
    ~TreeNotes();

private slots:
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
    void saveQSettings();
    void InitIconVector();
    void AddChildren(QDomDocument *doc ,QDomElement *elem, QTreeWidgetItem *parent);
    void saveToFile();
    void ReadFromFile();
    void ReadChildren(QDomDocument *doc, QDomNode node, TreeWidgetItem *parent);
    TreeWidgetItem* AddNote(TreeWidgetItem *parent, QString text, QString message = "", QIcon icon = QIcon(":/Resources/Icon.png"));
    void MoveUp(TreeWidgetItem *item);
    void MoveDown(TreeWidgetItem *item);
    void ShowContextMenu(const QPoint &pos);
    void CleanBackups(int max, QString backupsDir);
    void setStar(TreeWidgetItem*, bool);
    void ApplyMacroVector();

    QString boolToString(bool a);
    bool stringToBool(QString a);

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

private:
    Ui::TreeNotes *ui;

    app_config appConfig;

    TreeWidget *noteTree;
    QSplitter *splitter;

    QLabel *noteCntLabel;
    QLabel *childrenCntLabel;
    QLabel *dateTimeLabel;
    QLabel *lineCountLabel;
    QLabel *lengthLabel;
    QLabel *currentPositionLabel;

    bool doubleClickToEditMessage;
    QVector<QPair<QString, std::function<QString()>>> macroVec;

    QVector<UndoItem> undoVector;
};

#define SHORTCUT_JUMP_MSG "Ctrl+H"
#define SHORTCUT_JUMP_TITLE "Ctrl+J"

#endif // TREENOTES_H
