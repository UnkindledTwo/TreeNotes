#include "treenotes.h"
#include "ui_treenotes.h"

TreeNotes::TreeNotes(QWidget *parent)
   : QMainWindow(parent)
    , ui(new Ui::TreeNotes)
{
    qDebug() << "{";
    qDebug() << "--------------------";
    qDebug() << "Initializing window";
    ui->setupUi(this);

    this->setFocusPolicy(Qt::StrongFocus);
    this->setFocus();

    //Make an app config
    struct app_config appConfig;
    appConfig.notetree_alternating_row_colors = true;
    appConfig.notetree_indentation_size = 20;
    appConfig.opacity = 100;
    appConfig.treeWidgetAnimated = true;
    appConfig.doubleClickToEditMessage = true;
    appConfig.tabSize = 4;

    noteTree = ui->treeWidget;
    noteTree->clear();

    /*Init the splitter*/
    QSplitter *splitter = new QSplitter(this);
    splitter->addWidget(noteTree);
    QWidget* wrapper = new QWidget();
    ui->verticalLayout->setMargin(0);
    ui->verticalLayout->setParent(NULL);
    wrapper->setLayout(ui->verticalLayout);
    splitter->addWidget(wrapper);
    setCentralWidget(splitter);

    //Auto timer for refreshing label(s)
    QTimer *timer = new QTimer(this);
    timer->setInterval(500);
    connect(timer, &QTimer::timeout, [&](){RefreshLabels();});
    timer->start();

    //(Windows only) set borders of noteTree, messageEdit and titleEdit to the accent color of Windows 10
    QString styleSheetFocus = "border: 1px solid " + QtWin::colorizationColor().name() + ";";
    ui->messageEdit->setStyleSheet(ui->messageEdit->styleSheet() + styleSheetFocus);
    ui->treeWidget->setStyleSheet(ui->treeWidget->styleSheet() + styleSheetFocus);
    ui->titleEdit->setStyleSheet(ui->titleEdit->styleSheet() + styleSheetFocus);

    //Connect save and load from disk actions to an existing slot
    connect(ui->actionSave_To_Disk, &QAction::triggered, this, &TreeNotes::saveToFile);
    connect(ui->actionLoad_From_Disk, &QAction::triggered, this, &TreeNotes::ReadFromFile);

    InitIconVector();
    InitShortcuts();
    InitStatusLabels();
    ReadFromFile();
    ReadAppConfig(appConfig);
    ReadQSettings();


    qDebug() << "Initilization of the main window is finished";

}

TreeNotes::~TreeNotes()
{
    delete ui;
}

void TreeNotes::ReadQSettings(){
    qDebug() << "Reading QSettings";

    //QSettings settings("Unkindled", "NoteTree", this);
    QSettings settings("settings.ini", QSettings::IniFormat);
    QFont loadedFont = qvariant_cast<QFont>(settings.value("Text_Editor_Font", this->font()));
    ui->messageEdit->setFont(loadedFont);
    ui->titleEdit->setFont(loadedFont);

    //Set window size
    if(settings.value("maximized", false).toBool()){
        this->showMaximized();
    }
    else{
        this->setGeometry(qvariant_cast<QRect>(settings.value("geometry", this->geometry())));
    }

    //Set splitter sizes
    if(settings.value("noteTreeHidden", false).toBool()){
        on_actionHide_Show_Note_Tree_triggered();
    }
    else{
        ((QSplitter*)noteTree->parent())->setSizes({settings.value("s1", this->width() / 2).toInt(), settings.value("s2", this->width() / 2).toInt()});
    }

    qDebug() << "Reading QSettings is finished, file: " << settings.fileName();
}

void TreeNotes::saveQSettings(){
    qDebug() << "Saving QSettings";
    //QSettings settings("Unkindled", "NoteTree", this);
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.setValue("Text_Editor_Font", ui->messageEdit->font());
    settings.setValue("maximized", this->isMaximized());
    settings.setValue("geometry", this->geometry());
    settings.setValue("noteTreeHidden", noteTree->isHidden());
    settings.setValue("s1", ((QSplitter*)noteTree->parent())->sizes().at(0));
    settings.setValue("s2", ((QSplitter*)noteTree->parent())->sizes().at(1));
    qDebug() << "Saved QSettings, file: " << settings.fileName();
}

QString TreeNotes::dateTimeNoSpace(){
    QString res = "";
    res.append(QString::number(QDate::currentDate().year()));
    res.append("_");
    res.append(QString::number(QDate::currentDate().month()));
    res.append("_");
    res.append(QString::number(QDate::currentDate().day()));
    res.append(",");
    res.append(QString::number(QTime::currentTime().hour()));
    res.append(".");
    res.append(QString::number(QTime::currentTime().minute()));
    res.append(".");
    res.append(QString::number(QTime::currentTime().second()));

    return res;
}

void TreeNotes::AttemptSaveBackup(){
    qDebug() << "AttempSaveBackup()";
    QDir currentDir(qApp->applicationDirPath());
    QDir dir(qApp->applicationDirPath() + "/backup/");
    dir.mkdir(qApp->applicationDirPath() + "/backup/");
    currentDir.rename(qApp->applicationDirPath() + "/save.xml", qApp->applicationDirPath() + "/backup/save" + dateTimeNoSpace() + ".xml");

    if(QFile(qApp->applicationDirPath() + "/save.xml").exists()) qCritical().noquote() << "\n!!!Backup Failed!!!\n";
    qDebug() << "AttempSaveBackup() finished";
}

void TreeNotes::ReadAppConfig(app_config appConfig){
    noteTree->setIndentation(appConfig.notetree_indentation_size);
    noteTree->setAlternatingRowColors(appConfig.notetree_alternating_row_colors);
    this->setWindowOpacity(qreal(appConfig.opacity)/100);
    noteTree->setAnimated(appConfig.treeWidgetAnimated);
    doubleClickToEditMessage = appConfig.doubleClickToEditMessage;
    ui->messageEdit->setTabStopWidth(appConfig.tabSize);

    qDebug() << "App config read finished";
}

void TreeNotes::closeEvent(QCloseEvent *e){
    qDebug().noquote() << "\n\n--------------------\nClose Event" << e;
    AttemptSaveBackup();
    saveToFile();
    saveQSettings();
    qDebug() << "Closed window";
    qDebug() << "--------------------";
    qDebug() << "}";
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
        newItem->lastEdited = QDateTime::fromString(node.toElement().childNodes().at(i).toElement().attribute("lastEdited"));
        parent->addChild(newItem);
        ReadChildren(doc, node.toElement().childNodes().at(i), newItem);
    }
}

void TreeNotes::ReadFromFile(){
    qDebug() << "Reading from file";
    noteTree->clear();

    QString errMsg;
    int errLine;
    int errColumn;

    QDomDocument document;
    QFile file(qApp->applicationDirPath()+"/save.xml");
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

    QDomNodeList childElementList = document.childNodes();
    for(int j = 0; j < childElementList.count(); j++){
        QDomElement root = childElementList.at(j).toElement();
        QDomNodeList itemList = root.elementsByTagName("NoteItem");
        for(int i = 0; i < root.toElement().childNodes().count(); i++){
            if(!itemList.at(i).toElement().hasAttribute("message")) continue;
            if(itemList.at(i).toElement().attribute("title") == "") continue;

            TreeWidgetItem *newItem = new TreeWidgetItem();
            newItem->setIcon(0 , iconVector.at(root.toElement().childNodes().at(i).toElement().attribute("icon").toInt()));
            newItem->iconVectorIndex = (root.toElement().childNodes().at(i).toElement().attribute("icon").toInt());
            newItem->lastEdited = QDateTime::fromString(root.toElement().childNodes().at(i).toElement().attribute("lastEdited"));
            noteTree->addTopLevelItem(newItem);
            ReadChildren(&document, root.toElement().childNodes().at(i), newItem);
        }
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
            newElem.setAttribute("lastEdited", ((TreeWidgetItem*)(*it))->lastEdited.toString());
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
            elem.setAttribute("lastEdited", ((TreeWidgetItem*)(*it))->lastEdited.toString());
            AddChildren(&document, &elem, (*it));
            root.appendChild(elem);
        }
        it++;
    }

    QFile file(qApp->applicationDirPath()+"/save.xml");
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

TreeWidgetItem* TreeNotes::AddNote(TreeWidgetItem *parent, QString text,QString message, QIcon icon){
    TreeWidgetItem *itemToAdd = new TreeWidgetItem();
    itemToAdd->setIcon(0,icon);
    itemToAdd->setText(0,text);
    itemToAdd->iconVectorIndex = 0;
    itemToAdd->message = message;

    if(parent == NULL){
        noteTree->addTopLevelItem(itemToAdd);
        return itemToAdd;
    }

    parent->addChild(itemToAdd);
    parent->setExpanded(true);

    return itemToAdd;
}

void TreeNotes::Save(TreeWidgetItem *target){
    if(ui->messageEdit->toPlainText() != target->message || ui->titleEdit->text() != target->text(0)){
        target->lastEdited = QDateTime::currentDateTime();
    }
    target->message = ui->messageEdit->toPlainText();
    target->setText(0, ui->titleEdit->text());
}

void TreeNotes::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    qDebug() << "Item changed";
    if(!current) return;

    TreeWidgetItem *previousItem = (TreeWidgetItem*)previous;
    if(!previous || !previousItem) goto part2;
    Save(previousItem);

part2:
    TreeWidgetItem *currentItem = (TreeWidgetItem*)current;
    if(!currentItem){return;}
    ui->messageEdit->setPlainText(currentItem->message);
    ui->titleEdit->setText(currentItem->text(0));
}


void TreeNotes::on_actionAdd_triggered()
{
    AddNote((TreeWidgetItem*)noteTree->currentItem(), "New Note", "", iconVector.at(0));
}


void TreeNotes::on_actionSave_triggered()
{
    if(!noteTree->currentItem()) return;
    Save((TreeWidgetItem*)noteTree->currentItem());
}

void TreeNotes::Delete(QTreeWidgetItem *target){
    qDebug() << "Started deleting item";
    if(!target){
        qDebug() << "Found no item, return";
        return;
    }
    qDebug() << "Parent of selected item: "  << target->parent();
    QTreeWidgetItem *currentItem = target;
    if(currentItem->parent() != 0x0){
        target->parent()->removeChild(noteTree->currentItem());
    }
    else if(!currentItem->parent()){
        qDebug() << "No parent found";
        noteTree->takeTopLevelItem(noteTree->indexOfTopLevelItem(noteTree->currentItem()));
    }
    qDebug() << "Deleted item";
}

void TreeNotes::on_actionDelete_triggered()
{
    Delete(noteTree->currentItem());
}

void TreeNotes::MoveUp(TreeWidgetItem *item){
    TreeWidgetItem *i = (TreeWidgetItem*)noteTree->currentItem();
    bool expanded = i->isExpanded();
    int row = noteTree->currentIndex().row();
    if(item && item->parent() && row > 0){
        TreeWidgetItem *parent = (TreeWidgetItem*)i->parent();
        int index = parent->indexOfChild(i);
        TreeWidgetItem *child = (TreeWidgetItem*)parent->takeChild(index);
        parent->insertChild(index-1, child);

    }
    else if(item && row > 0){
        int index = noteTree->indexOfTopLevelItem(i);
        TreeWidgetItem *child = (TreeWidgetItem*)noteTree->takeTopLevelItem(index);
        noteTree->insertTopLevelItem(index -1, child);

    }

    noteTree->setCurrentItem(i);
    i->setExpanded(expanded);
}

void TreeNotes::MoveDown(TreeWidgetItem *item){
    TreeWidgetItem *i = (TreeWidgetItem*)noteTree->currentItem();
    bool expanded = i->isExpanded();
    int row = noteTree->currentIndex().row();
    if(item && item->parent() && row < i->parent()->childCount()-1){
        TreeWidgetItem *parent = (TreeWidgetItem*)i->parent();
        int index = parent->indexOfChild(i);
        TreeWidgetItem *child = (TreeWidgetItem*)parent->takeChild(index);
        parent->insertChild(index+1, child);

    }
    else if(item && row < noteTree->topLevelItemCount()-1){
        int index = noteTree->indexOfTopLevelItem(i);
        TreeWidgetItem *child = (TreeWidgetItem*)noteTree->takeTopLevelItem(index);
        noteTree->insertTopLevelItem(index+1, child);

    }

    noteTree->setCurrentItem(i);
    i->setExpanded(expanded);
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
    iconVector.append(QIcon(":/Resources/Icon.ico"));
    iconVector.append(style()->standardIcon(QStyle::SP_FileIcon));
    iconVector.append(style()->standardIcon(QStyle::SP_DirIcon));
    iconVector.append(style()->standardIcon(QStyle::SP_MessageBoxWarning));
    iconVector.append(style()->standardIcon(QStyle::SP_MessageBoxCritical));
    iconVector.append(style()->standardIcon(QStyle::SP_ComputerIcon));
    iconVector.append(style()->standardIcon(QStyle::SP_DirHomeIcon));
    iconVector.append(style()->standardIcon(QStyle::SP_FileLinkIcon));
    iconVector.append(style()->standardIcon(QStyle::SP_TrashIcon));
    iconVector.append(style()->standardIcon(QStyle::SP_DialogSaveButton));
    iconVector.append(style()->standardIcon(QStyle::SP_FileDialogNewFolder));
    iconVector.append(style()->standardIcon(QStyle::SP_FileDialogStart));
    iconVector.append(style()->standardIcon(QStyle::SP_DialogApplyButton));
    iconVector.append(style()->standardIcon(QStyle::SP_DesktopIcon));
    iconVector.append(style()->standardIcon(QStyle::SP_VistaShield));
    iconVector.append(style()->standardIcon(QStyle::SP_ArrowBack));
}

void TreeNotes::InitShortcuts(){
    QShortcut *jumpToMessageBox = new QShortcut(QKeySequence(SHORTCUT_JUMP_MSG), this);
    connect(jumpToMessageBox, &QShortcut::activated, [&](){ui->messageEdit->setFocus();});

    QShortcut *jumpToTitleBox = new QShortcut(QKeySequence(SHORTCUT_JUMP_TITLE), this);
    connect(jumpToTitleBox, &QShortcut::activated, [&](){ui->titleEdit->setFocus();});

    QShortcut *jumpToNoteTree = new QShortcut(QKeySequence(SHORTCUT_JUMP_TREE), this);
    connect(jumpToNoteTree, &QShortcut::activated, [&](){noteTree->setFocus();
        if(noteTree->topLevelItemCount() > 0 && noteTree->selectedItems().count() == 0)
            noteTree->topLevelItem(0)->setSelected(true);
    }
    );
}

void TreeNotes::InitStatusLabels(){
    qDebug() << "Init status labels";

    QFrame *line;
    line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    noteCntLabel = new QLabel(this);
    noteCntLabel->setScaledContents(true);
    childrenCntLabel = new QLabel(this);
    childrenCntLabel->setScaledContents(true);
    dateTimeLabel = new QLabel(this);
    dateTimeLabel->setScaledContents(true);

    ui->statusbar->addWidget(noteCntLabel);
    ui->statusbar->addWidget(line);
    ui->statusbar->addWidget(childrenCntLabel);
    ui->statusbar->addWidget(line);
    ui->statusbar->addWidget(dateTimeLabel);


    qDebug() << "Init status labels finished";
}

void TreeNotes::RefreshLabels(){
    noteCntLabel->setText("Note count: "+ QString::number(NoteCount()));
    if(noteTree->currentItem()){
        childrenCntLabel->setText("Children count: "+ QString::number(noteTree->currentItem()->childCount()));
        dateTimeLabel->setText("Last edited: "+((TreeWidgetItem*)noteTree->currentItem())->lastEdited.toString());
    }
    else{
        childrenCntLabel->setText("Children count: 0");
        dateTimeLabel->setText("Last edited: ");
    }
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

void TreeNotes::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    qDebug() << "Item double clicked" << item << "column" << column;
    if(doubleClickToEditMessage)
        ui->titleEdit->setFocus();
}


void TreeNotes::on_actionQt_triggered()
{
    QMessageBox::aboutQt(this);
}


void TreeNotes::on_actionSet_Font_Text_Editors_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, ui->messageEdit->font(), this, "Select a Text Editor Font");
    if(ok){
        ui->titleEdit->setFont(font);
        ui->messageEdit->setFont(font);
    }
}

QString TreeNotes::boolToString(bool a){
    if(a){
        return "true";
    }
    if(!a){
        return "false";
    }
    return "";
}

bool TreeNotes::stringToBool(QString a){
    return a == "true";
}
