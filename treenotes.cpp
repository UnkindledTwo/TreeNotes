#include "treenotes.h"
#include "ui_treenotes.h"

TreeNotes::TreeNotes(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TreeNotes)
{
    ui->setupUi(this);

    this->setFocusPolicy(Qt::StrongFocus);
    this->setFocus();

    struct app_config appConfig;
    appConfig.notetree_alternating_row_colors = true;
    appConfig.notetree_indentation_size = 20;

    noteTree = ui->treeWidget;
    noteTree->setIndentation(appConfig.notetree_indentation_size);
    noteTree->setAlternatingRowColors(appConfig.notetree_alternating_row_colors);
    noteTree->clear();

    InitIconVector();
    InitShortcuts();
    InitStatusLabels();
    readFromFile();

    /*Init the splitter*/
    QSplitter *splitter = new QSplitter(this);
    splitter->addWidget(noteTree);
    QWidget* wrapper = new QWidget();
    ui->verticalLayout->setMargin(0);
    ui->verticalLayout->setParent(NULL);
    wrapper->setLayout(ui->verticalLayout);
    splitter->addWidget(wrapper);
    setCentralWidget(splitter);

    QTimer *timer = new QTimer(this);
    timer->setInterval(500);
    connect(timer, &QTimer::timeout, [&](){RefreshLabels();});
    timer->start();

    connect(ui->actionSave_To_Disk, &QAction::triggered, this, &TreeNotes::saveToFile);
    connect(ui->actionLoad_From_Disk, &QAction::triggered, this, &TreeNotes::readFromFile);

}

TreeNotes::~TreeNotes()
{
    delete ui;
}

void TreeNotes::closeEvent(QCloseEvent *e){
    saveToFile();
}

void TreeNotes::ReadChildren(QDomDocument *doc, QDomNode node, TreeWidgetItem *parent){
    if(!node.isElement()) return;
    if(!node.toElement().hasAttribute("message")) return;
    if(node.toElement().attribute("title") == "") return;
    if(node.toElement().tagName() == "") return;
    parent->message = node.toElement().attribute("message");
    parent->setText(0,node.toElement().attribute("title"));

    QDomNodeList itemList = node.toElement().elementsByTagName("NoteItem");
    for(int i = 0; i < node.toElement().childNodes().count(); i++){
        TreeWidgetItem *newItem = new TreeWidgetItem();
        newItem->setIcon(0 , iconVector.at(node.toElement().childNodes().at(i).toElement().attribute("icon").toInt()));
        newItem->iconVectorIndex = node.toElement().childNodes().at(i).toElement().attribute("icon").toInt();
        parent->addChild(newItem);
        ReadChildren(doc, node.toElement().childNodes().at(i), newItem);
    }
}

void TreeNotes::readFromFile(){
    qDebug() << "Reading from file";
    QString errMsg;
    int errLine;
    int errColumn;

    QDomDocument document;
    QFile file("save.xml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // Error while loading file
        qDebug() << "Error while loading file";
    }
    if(!document.setContent(file.readAll(), false, &errMsg, &errLine, &errColumn)){
        qDebug() << "Error while setting xmldoc content: " << errMsg << QString::number(errLine) << QString::number(errColumn);
    }
    file.close();
    qDebug().noquote()<< document.toString(4);


    QDomElement root = document.firstChildElement();
    QDomNodeList itemList = root.elementsByTagName("NoteItem");
    for(int i = 0; i < root.toElement().childNodes().count(); i++){
        if(!itemList.at(i).toElement().hasAttribute("message")) continue;
        if(itemList.at(i).toElement().attribute("title") == "") continue;

        TreeWidgetItem *newItem = new TreeWidgetItem();
        newItem->setIcon(0 , iconVector.at(itemList.at(i).toElement().attribute("icon").toInt()));
        newItem->iconVectorIndex = itemList.at(i).toElement().attribute("icon").toInt();
        noteTree->addTopLevelItem(newItem);
        ReadChildren(&document, root.toElement().childNodes().at(i), newItem);
    }
    qDebug() << "Reading from file finished";
}

void TreeNotes::AddChildren(QDomDocument *doc, QDomElement* elem, QTreeWidgetItem *parent){
    QTreeWidgetItemIterator it(noteTree);
    while(*it){
        if((*it)->parent() == parent){
            QDomElement newElem = doc->createElement("NoteItem");
            newElem.setAttribute("message", ((TreeWidgetItem*)(*it))->message);
            newElem.setAttribute("title", ((TreeWidgetItem*)(*it))->text(0));
            newElem.setAttribute("icon", ((TreeWidgetItem*)(*it))->iconVectorIndex);
            elem->appendChild(newElem);
            AddChildren(doc, &newElem, *it);
        }
        it++;
    }
}

void TreeNotes::saveToFile(){
    qDebug() << "Saving to file";
    QDomDocument document;

    QDomElement root = document.createElement("All_Notes");

    document.appendChild(root);

    QTreeWidgetItemIterator it(noteTree);
    while(*it){
        if((*it)->parent() == 0x0){

            QDomElement elem = document.createElement("NoteItem");
            elem.setAttribute("message", ((TreeWidgetItem*)(*it))->message);
            elem.setAttribute("title", (*it)->text(0));
            elem.setAttribute("icon", ((TreeWidgetItem*)(*it))->iconVectorIndex);
            AddChildren(&document, &elem, (*it));
            root.appendChild(elem);
        }
        it++;
        /*
        if((*it)->parent() == 0x0){
            QTreeWidgetItem *current = *it;
            QDomElement treeItemElement;
            treeItemElement = document.createElement(current->text(0));
            root.appendChild(treeItemElement);

            if(current->childCount() > 0){
                QTreeWidgetItemIterator it1(noteTree);
                while (*it1){
                    if((*it1)->parent() == current){
                        treeItemElement.appendChild(document.createElement((*it)->text(0)));
                    }
                    it1++;
                }
            }


            it++;
        }
        else it++;
        */
    }
    QFile file("save.xml");
    file.open(QIODevice::WriteOnly);
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    stream << document.toString(4);
    file.close();
    stream.flush();

    qDebug().noquote() << document.toString();
    qDebug() << "Saving to file finished";
}

void TreeNotes::keyPressEvent(QKeyEvent *e){
    if(e->key() == Qt::Key_Return){
        if(noteTree->currentItem() && noteTree->hasFocus()){
            noteTree->currentItem()->setExpanded(!noteTree->currentItem()->isExpanded());
        }
    }
}

void TreeNotes::AddNote(TreeWidgetItem *parent, QString text,QString message, QIcon icon){
    TreeWidgetItem *itemToAdd = new TreeWidgetItem();
    itemToAdd->setIcon(0,icon);
    itemToAdd->setText(0,text);
    itemToAdd->message = message;

    if(parent == NULL){
        noteTree->addTopLevelItem(itemToAdd);
        return;
    }

    parent->addChild(itemToAdd);
    parent->setExpanded(true);
}

void TreeNotes::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    qDebug() << "Item changed";
    if(!current) return;

    TreeWidgetItem *previousItem = (TreeWidgetItem*)previous;
    if(!previous || !previousItem) goto part2;
    previousItem->message = ui->messageEdit->toPlainText();
    previousItem->setText(0,ui->titleEdit->text());

    part2:
    TreeWidgetItem *currentItem = (TreeWidgetItem*)current;
    if(!currentItem){return;}
    ui->messageEdit->setPlainText(currentItem->message);
    ui->titleEdit->setText(currentItem->text(0));
}


void TreeNotes::on_actionAdd_triggered()
{
    AddNote((TreeWidgetItem*)noteTree->currentItem(), "New Note", "", iconVector.at(6));
}


void TreeNotes::on_actionSave_triggered()
{
    if(!noteTree->currentItem()) return;
    ((TreeWidgetItem*)noteTree->currentItem())->message = ui->messageEdit->toPlainText();
    ((TreeWidgetItem*)noteTree->currentItem())->setText(0, ui->titleEdit->text());
}


void TreeNotes::on_actionDelete_triggered()
{
    qDebug() << "Started deleting item";
    if(!noteTree->currentItem()) return;
    qDebug() << noteTree->currentItem()->parent();
    if(noteTree->currentItem()->parent() != 0x0){
        noteTree->currentItem()->parent()->removeChild(noteTree->currentItem());
    }
    else if(noteTree->currentItem()->parent() == 0x0){
        qDebug() << "No parent found";
        noteTree->takeTopLevelItem(noteTree->indexOfTopLevelItem(noteTree->currentItem()));
    }
    qDebug() << "Deleted item";
}

void TreeNotes::MoveUp(TreeWidgetItem *item){
    TreeWidgetItem *i = (TreeWidgetItem*)noteTree->currentItem();
    int row = noteTree->currentIndex().row();
    if(item && item->parent() && row > 0){
        TreeWidgetItem *parent = (TreeWidgetItem*)i->parent();
        int index = parent->indexOfChild(i);
        TreeWidgetItem *child = (TreeWidgetItem*)parent->takeChild(index);
        parent->insertChild(index-1, child);

        noteTree->setCurrentItem(child);
    }

}

void TreeNotes::MoveDown(TreeWidgetItem *item){
    TreeWidgetItem *i = (TreeWidgetItem*)noteTree->currentItem();
    int row = noteTree->currentIndex().row();
    if(item && item->parent() && row < i->parent()->childCount()-1){
        TreeWidgetItem *parent = (TreeWidgetItem*)i->parent();
        int index = parent->indexOfChild(i);
        TreeWidgetItem *child = (TreeWidgetItem*)parent->takeChild(index);
        parent->insertChild(index+1, child);

        noteTree->setCurrentItem(child);
    }
}

void TreeNotes::on_actionMove_Up_triggered()
{
    MoveUp((TreeWidgetItem*)noteTree->currentItem());
}


void TreeNotes::on_actionMove_Down_triggered()
{
    MoveDown((TreeWidgetItem*)noteTree->currentItem());
}


void TreeNotes::on_actionSet_Icon_triggered()
{
    if(!noteTree->currentItem()) return;
    IconSelectorDialog *isd = new IconSelectorDialog(NULL, iconVector);
    isd->setFont(this->font());
    if(isd->exec()){
        noteTree->currentItem()->setIcon(0, isd->selectedIcon);
        ((TreeWidgetItem*)noteTree->currentItem())->iconVectorIndex = isd->index;
        qDebug() << QString::number(isd->index);
    }
}

void TreeNotes::on_actionFocus_Parent_triggered()
{
    if(!noteTree->currentItem()) return;
    if(noteTree->currentItem()->parent()){
        noteTree->setCurrentItem(noteTree->currentItem()->parent());
    }
    else{
        noteTree->clearSelection();
    }
}


void TreeNotes::InitIconVector(){
    iconVector.append(style()->standardIcon(QStyle::SP_ArrowBack));
    iconVector.append(style()->standardIcon(QStyle::SP_FileDialogNewFolder));
    iconVector.append(style()->standardIcon(QStyle::SP_FileDialogStart));
    iconVector.append(style()->standardIcon(QStyle::SP_ComputerIcon));
    iconVector.append(style()->standardIcon(QStyle::SP_DialogApplyButton));
    iconVector.append(style()->standardIcon(QStyle::SP_DirIcon));
    iconVector.append(style()->standardIcon(QStyle::SP_FileIcon));
    iconVector.append(style()->standardIcon(QStyle::SP_FileLinkIcon));
    iconVector.append(style()->standardIcon(QStyle::SP_DesktopIcon));
    iconVector.append(style()->standardIcon(QStyle::SP_VistaShield));
    iconVector.append(style()->standardIcon(QStyle::SP_TrashIcon));
    iconVector.append(style()->standardIcon(QStyle::SP_MessageBoxCritical));
    iconVector.append(style()->standardIcon(QStyle::SP_MessageBoxWarning));
    iconVector.append(style()->standardIcon(QStyle::SP_DialogSaveButton));
    iconVector.append(style()->standardIcon(QStyle::SP_DirHomeIcon));

}

void TreeNotes::InitShortcuts(){
    QShortcut *jumpToMessageBox = new QShortcut(QKeySequence(SHORTCUT_JUMP_MSG), this);
    connect(jumpToMessageBox, &QShortcut::activated, [&](){ui->messageEdit->setFocus();});

    QShortcut *jumpToTitleBox = new QShortcut(QKeySequence(SHORTCUT_JUMP_TITLE), this);
    connect(jumpToTitleBox, &QShortcut::activated, [&](){ui->titleEdit->setFocus();});

    QShortcut *jumpToNoteTree = new QShortcut(QKeySequence(SHORTCUT_JUMP_TREE), this);
    connect(jumpToNoteTree, &QShortcut::activated, [&](){noteTree->setFocus();});
}

void TreeNotes::InitStatusLabels(){
    noteCntLabel = new QLabel(this);
    ui->statusbar->addWidget(noteCntLabel);

}

void TreeNotes::RefreshLabels(){
    noteCntLabel->setText("Note count: "+ QString::number(NoteCount()));
}

int TreeNotes::NoteCount(){
    int cnt = 0;
    QTreeWidgetItemIterator it(noteTree);
    while(*it){
        cnt++;
        *it++;
    }
    return cnt;
}

void TreeNotes::on_actionHide_Show_Note_Tree_triggered()
{
    if(noteTree->isHidden()){
        noteTree->show();
    }
    else{
        noteTree->hide();
    }
}
