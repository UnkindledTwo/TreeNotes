#ifndef TREENOTES_H
#define TREENOTES_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QListWidget>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QStyle>
#include <QShortcut>
#include <QMessageBox>
#include <QKeyEvent>
#include <QtXml>
#include <QTextStream>
#include <QLabel>
#include <QTimer>
#include <QSplitter>

#include <QtWinExtras>

#include "treewidgetitem.h"
#include "iconselectordialog.h"

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

    struct app_config{
        bool notetree_alternating_row_colors;
        int notetree_indentation_size;
        int opacity; //100 for solid, 0 for transparent
        bool treeWidgetAnimated;
    };

private slots:
    void ReadAppConfig(struct app_config appConfig);

    int NoteCount();

    void RefreshLabels();

    void InitStatusLabels();

    void InitShortcuts();

    void InitIconVector();

    void AddChildren(QDomDocument *doc ,QDomElement *elem, QTreeWidgetItem *parent);

    void saveToFile();

    void readFromFile();

    void ReadChildren(QDomDocument *doc, QDomNode node, TreeWidgetItem *parent);

    void AddNote(TreeWidgetItem *parent, QString text, QString message = "", QIcon icon = QIcon());

    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_actionAdd_triggered();

    void on_actionSave_triggered();

    void on_actionDelete_triggered();

    void MoveUp(TreeWidgetItem *item);

    void MoveDown(TreeWidgetItem *item);

    void on_actionMove_Up_triggered();

    void on_actionMove_Down_triggered();

    void on_actionSet_Icon_triggered();

    void on_actionFocus_Parent_triggered();

    void on_actionHide_Show_Note_Tree_triggered();

private:
    Ui::TreeNotes *ui;

    QTreeWidget *noteTree;

    QLabel *noteCntLabel;
};

#define SHORTCUT_JUMP_MSG "Ctrl+H"
#define SHORTCUT_JUMP_TREE "Ctrl+K"
#define SHORTCUT_JUMP_TITLE "Ctrl+J"

#endif // TREENOTES_H
