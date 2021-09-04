#include "treenotes.h"
#include "ui_treenotes.h"

TreeNotes::TreeNotes(QWidget *parent, QString saveFileName)
    : QMainWindow(parent)
    , ui(new Ui::TreeNotes)
{
    qDebug() << "{";
    qDebug() << "--------------------";
    qDebug() << "Initializing window";
    this->saveFileName = saveFileName;
    ui->setupUi(this);

    noteTree = ui->treeWidget;
    noteTree->clear();

    this->setFocusPolicy(Qt::StrongFocus);
    this->setFocus();

    //Initialize the splitter
    splitter = new QSplitter();
    splitter->addWidget(noteTree);


    QWidget* wrapper = new QWidget();
    ui->verticalLayout->setMargin(0);
    ui->verticalLayout->setParent(NULL);
    wrapper->setLayout(ui->verticalLayout);
    splitter->addWidget(wrapper);
    QGridLayout *grid = new QGridLayout();
    grid->addWidget(splitter);
    QWidget *central = new QWidget();
    central->setLayout(grid);
    this->setCentralWidget(central);

    //Auto timer for refreshing label(s)
    QTimer *timer = new QTimer(this);
    timer->setInterval(50);
    connect(timer, &QTimer::timeout, this,[&](){RefreshLabels();});
    timer->start();

    //Set borders of noteTree, messageEdit and titleEdit to the accent color of Windows
#ifdef Q_OS_WIN
    QString styleSheet = "border: 1.50px solid " + QtWin::colorizationColor().light().name() + ";";
    styleSheet += "border-radius: 2px;";
    styleSheet += "padding: 3px;";
    ui->treeWidget->setStyleSheet("QTreeWidget{" + styleSheet + "}");
    styleSheet += "selection-color: white;";
    styleSheet += "selection-background-color:" + QtWin::colorizationColor().name() + ";";
    ui->titleEdit->setStyleSheet("QLineEdit{" + styleSheet + "}");
    ui->messageEdit->setStyleSheet("QPlainTextEdit{" + styleSheet + "}");
#else
    QString styleSheet = "border: 1.50px solid black;";
    styleSheet += "border-radius: 2px;";
    styleSheet += "padding: 3px;";
    //ui->treeWidget->setStyleSheet("QTreeWidget{" + styleSheet + "}");
    //ui->titleEdit->setStyleSheet("QLineEdit{" + styleSheet + "}");
    //ui->messageEdit->setStyleSheet("QPlainTextEdit{" + styleSheet + "}");
#endif

    ReadQSettings();
    ReadAppConfig(appConfig);
    //Connect save and load from disk actions to an existing slot
    connect(ui->actionSave_To_Disk, &QAction::triggered, this, &TreeNotes::saveToFile);
    connect(ui->actionLoad_From_Disk, &QAction::triggered, this, &TreeNotes::ReadFromFile);

    //Context menu event
    noteTree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(noteTree, &QTreeWidget::customContextMenuRequested, this, &TreeNotes::ShowContextMenu);

    InitIconVector();
    InitShortcuts();
    InitStatusLabels();
    ReadFromFile();
    ReadQSettings();
    ReadAppConfig(appConfig);
    InitMacroVector();

    ui->messageEdit->setCursorWidth(2);
    ui->messageEdit->setSymbolHighlighting(true);

    //Make sure the noteTree's column sizes are accurate, kinda hacky
    noteTree->expandAll();
    noteTree->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    noteTree->resizeColumnToContents(1);
    noteTree->collapseAll();

    qDebug() << "Initilization of the main window is finished";
}

TreeNotes::~TreeNotes()
{
    delete ui;
}

void TreeNotes::ShowContextMenu(const QPoint &pos){
    //Context Menu Event
    if(!noteTree->rect().contains(pos)){
        return;
    }
    QMenu contextMenu(tr("Context Menu"), this);

    contextMenu.addAction(ui->actionAdd);
    contextMenu.addAction(ui->actionSave);
    contextMenu.addAction(ui->actionDelete);
    contextMenu.addAction(ui->actionUndo_Delete);
    contextMenu.addAction(ui->actionSet_Icon);
    contextMenu.addAction(ui->actionMove_Up);
    contextMenu.addAction(ui->actionMove_Down);
    contextMenu.addAction(ui->actionFocus_Parent);
    contextMenu.addAction(ui->actionStar_Unstar);
    contextMenu.addAction(ui->actionRead_Only);
    contextMenu.addAction(ui->actionCollapse_All);
    contextMenu.addAction(ui->actionExpand_All);

    //contextMenu.exec(mapToGlobal(pos));
    contextMenu.exec(QCursor().pos());
}

void TreeNotes::ReadQSettings(){
    qDebug() << "Reading QSettings";

    //QSettings settings("Unkindled", "NoteTree", this);
    QSettings settings("settings.ini", QSettings::IniFormat);

    //Read the saved font
    QFont loadedFont = qvariant_cast<QFont>(settings.value("Text_Editor_Font", this->font()));
    ui->messageEdit->setFont(loadedFont);
    ui->titleEdit->setFont(loadedFont);

    ui->ToolBar->setHidden(settings.value("toolbar_hidden", false).toBool());
    ui->titleEdit->setHidden(settings.value("title_hidden", false).toBool());

    //Set window size
    if(settings.value("fullscreen", false).toBool()){
        this->showFullScreen();
    }
    if(settings.value("maximized", false).toBool()){
        this->showMaximized();
    }
    else{
        this->setGeometry(qvariant_cast<QRect>(settings.value("geometry", this->geometry())));
    }

    //Set splitter sizes
    if(settings.value("noteTreeHidden", false).toBool()){
        on_actionHide_Show_Note_Tree_triggered();
        ui->actionHide_Show_Note_Tree->setChecked(true);
    }
    else{
        ((QSplitter*)noteTree->parent())->setSizes({settings.value("s1", this->width() / 2).toInt(), settings.value("s2", this->width() / 2).toInt()});
    }

    //Appconfig section, these can be editable from settings.ini or from the application
    settings.beginGroup("AppConfig");

    appConfig.notetree_select_new_items = settings.value("notetree_select_new_items", appConfig.notetree_select_new_items).toBool();
    appConfig.notetree_alternating_row_colors = settings.value("notetree_alternating_row_colors", appConfig.notetree_alternating_row_colors).toBool();
    appConfig.notetree_indentation_size = settings.value("notetree_indentation_size", appConfig.notetree_indentation_size).toInt();
    appConfig.opacity = settings.value("opacity", appConfig.opacity).toInt();
    appConfig.notetree_animated = settings.value("notetree_animated", appConfig.notetree_animated).toBool();
    appConfig.doubleClickToEditMessage = settings.value("doubleClickToEditMessage", appConfig.doubleClickToEditMessage).toBool();
    appConfig.layoutMargin = settings.value("layoutMargin", appConfig.layoutMargin).toInt();
    appConfig.splitter_handle_width = settings.value("splitter_handle_width", appConfig.splitter_handle_width).toInt();
    appConfig.confirm_delete = settings.value("confirm_delete", appConfig.confirm_delete).toBool();
    appConfig.line_wrapping = settings.value("line_wrapping", appConfig.line_wrapping).toBool();
    appConfig.pair_completion = settings.value("pair_completion", appConfig.pair_completion).toBool();
    appConfig.notetree_drag_drop = settings.value("notetree_drag_drop", appConfig.notetree_drag_drop).toBool();
    appConfig.maximum_backups = settings.value("maximum_backups", appConfig.maximum_backups).toInt();
    appConfig.highlight_current_line = settings.value("highlight_current_line", appConfig.highlight_current_line).toBool();
    appConfig.use_native_theme = settings.value("use_native_theme", appConfig.use_native_theme).toBool();
    appConfig.dark_mode = settings.value("dark_mode", appConfig.dark_mode).toBool();

    settings.endGroup();
    ReadAppConfig(appConfig);

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
    settings.setValue("toolbar_hidden", ui->ToolBar->isHidden());
    settings.setValue("title_hidden", ui->titleEdit->isHidden());
    settings.setValue("fullscreen", this->isFullScreen());


    settings.beginGroup("AppConfig");

    settings.setValue("notetree_select_new_items", appConfig.notetree_select_new_items);
    settings.setValue("notetree_alternating_row_colors", appConfig.notetree_alternating_row_colors);
    settings.setValue("notetree_indentation_size", appConfig.notetree_indentation_size);
    settings.setValue("opacity", appConfig.opacity);
    settings.setValue("notetree_animated", appConfig.notetree_animated);
    settings.setValue("doubleClickToEditMessage", appConfig.doubleClickToEditMessage);
    settings.setValue("layoutMargin", appConfig.layoutMargin);
    settings.setValue("splitter_handle_width", appConfig.splitter_handle_width);
    settings.setValue("confirm_delete", appConfig.confirm_delete);
    settings.setValue("line_wrapping", appConfig.line_wrapping);
    settings.setValue("pair_completion", appConfig.pair_completion);
    settings.setValue("notetree_drag_drop", appConfig.notetree_drag_drop);
    settings.setValue("maximum_backups", appConfig.maximum_backups);
    settings.setValue("highlight_current_line", appConfig.highlight_current_line);
    settings.setValue("use_native_theme", appConfig.use_native_theme);
    settings.setValue("dark_mode", appConfig.dark_mode);

    settings.endGroup();

    qDebug() << "Saved QSettings, file: " << settings.fileName();
}

QString TreeNotes::dateTimeNoSpace(){
    //I don't know what I was thinkink with this, but it works
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
    QFileInfo backupFileInfo(qApp->applicationDirPath() + "/backup/" + saveFileName + dateTimeNoSpace() + ".xml");
    QDir backupDir(backupFileInfo.absoluteDir().path());
    qDebug() << backupFileInfo.absoluteDir().path();
    if(!backupDir.exists()) {
        qDebug() << backupDir.absolutePath();
        backupDir.mkpath(backupDir.absolutePath());
    }

    //Move the current save file to the backups directory and rename it to save{date}.xml
    backupDir.rename(qApp->applicationDirPath() + "/" + saveFileName, backupFileInfo.absoluteFilePath());

    //If the save can't be moved
    if(QFile(qApp->applicationDirPath() + "/save.xml").exists())
        qCritical().noquote() << "\n!!!Backup Failed!!!\n";

    CleanBackups(appConfig.maximum_backups, backupDir.absolutePath());
    qDebug() << "AttempSaveBackup() finished";
}

void TreeNotes::ReadAppConfig(app_config appConfig){
    noteTree->setIndentation(appConfig.notetree_indentation_size);
    noteTree->setAlternatingRowColors(appConfig.notetree_alternating_row_colors);
    this->setWindowOpacity(qreal(appConfig.opacity)/100);
    noteTree->setAnimated(appConfig.notetree_animated);
    doubleClickToEditMessage = appConfig.doubleClickToEditMessage;
    centralWidget()->layout()->setMargin(appConfig.layoutMargin);
    splitter->setHandleWidth(appConfig.splitter_handle_width);
    if(appConfig.line_wrapping){
        ui->messageEdit->setLineWrapMode(PlainTextEdit::WidgetWidth);
    }
    else{
        ui->messageEdit->setLineWrapMode(PlainTextEdit::NoWrap);
    }
    ui->messageEdit->setPairCompletion(appConfig.pair_completion);
    noteTree->setDragDrop(appConfig.notetree_drag_drop);
    ui->messageEdit->setLineHighlighting(appConfig.highlight_current_line);
    if(appConfig.use_native_theme){
        qApp->setStyleSheet("");
        #ifdef Q_OS_LINUX
            qApp->setStyle(QStyleFactory::create("gtk2"));
        #endif
        goto nativeTheme;
    }
    if(appConfig.dark_mode){
        QFile file(":/dark-green/stylesheet.qss");
        file.open(QFile::ReadOnly | QFile::Text);
        QTextStream stream(&file);
        qApp->setStyleSheet(stream.readAll());
    }
    else{
        QFile file(":/light/stylesheet.qss");
        file.open(QFile::ReadOnly | QFile::Text);
        QTextStream stream(&file);
        qApp->setStyleSheet(stream.readAll());
    }


    nativeTheme:

    qDebug() << "App config read finished";
}

void TreeNotes::closeEvent(QCloseEvent *e){
    qDebug().noquote() << "\n\n--------------------\nClose Event" << e;
    //Save the selected item
    if(noteTree->currentItem()){
        Save((TreeWidgetItem*)noteTree->currentItem());
    }

    AttemptSaveBackup();
    saveToFile();
    saveQSettings();
    qDebug() << "Closed window";
    qDebug() << "--------------------";
    qDebug() << "}";
}

void TreeNotes::ReadFromFile(){
    Saver s(this->noteTree, this->iconVector);
    s.saveFileName = this->saveFileName;
    s.ReadFromFile();
}

void TreeNotes::saveToFile(){
    Saver s(this->noteTree, this->iconVector);
    s.saveFileName = this->saveFileName;
    s.SaveToFile();
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
        //New Item has no parent
        noteTree->addTopLevelItem(itemToAdd);
        goto topLevel;
    }

    parent->addChild(itemToAdd);
    parent->setExpanded(true);

    topLevel:
    if(appConfig.notetree_select_new_items)
    noteTree->setCurrentItem(itemToAdd);

    return itemToAdd;
}

void TreeNotes::Save(TreeWidgetItem *target){
    if(ui->messageEdit->toPlainText() == target->message && ui->titleEdit->text() == target->text(0)){
        return;
    }

    target->lastEdited = QDateTime::currentDateTime();

    int savedPos = ui->messageEdit->textCursor().position();

    ApplyMacroVector();

    target->message = ui->messageEdit->toPlainText();
    target->setText(0, ui->titleEdit->text());

    target->hScrollbarPos = ui->messageEdit->horizontalScrollBar()->value();
    target->vScrollbarPos = ui->messageEdit->verticalScrollBar()->value();
    target->cursorpos = savedPos;

    on_treeWidget_currentItemChanged(noteTree->currentItem(), NULL);

    if(savedPos > target->message.length()){
        savedPos = target->message.length();
    }

    QTextCursor a = ui->messageEdit->textCursor();
    a.setPosition(savedPos);
    ui->messageEdit->setTextCursor(a);
}

void TreeNotes::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    qDebug() << "Item changed";
    if(!current){
        ui->messageEdit->clear();
        ui->titleEdit->clear();
        return;
    }

    TreeWidgetItem *previousItem = (TreeWidgetItem*)previous;
    if(!previous || !previousItem) goto part2;
    Save(previousItem);

part2:
    TreeWidgetItem *currentItem = (TreeWidgetItem*)current;
    if(!currentItem){return;}

    ui->messageEdit->fastSetPlainText(currentItem->message);
    ui->titleEdit->setText(currentItem->text(0));

    ui->messageEdit->verticalScrollBar()->setValue(currentItem->vScrollbarPos);
    ui->messageEdit->horizontalScrollBar()->setValue(currentItem->hScrollbarPos);
    ui->messageEdit->setPosition(currentItem->cursorpos);

    if(currentItem->readOnly != ui->messageEdit->isReadOnly()){
        ui->messageEdit->setReadOnly(currentItem->readOnly);
        ui->titleEdit->setReadOnly(currentItem->readOnly);
        ui->actionRead_Only->setChecked(currentItem->readOnly);
    }
}


void TreeNotes::on_actionAdd_triggered()
{
    AddNote((TreeWidgetItem*)noteTree->currentItem(), "New Note", "");
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

    UndoItem undoitem;
    undoitem.item = target;
    undoitem.parent = target->parent();

    if(currentItem->parent() != 0x0){
        undoitem.position = target->parent()->indexOfChild(target);
        target->parent()->removeChild(noteTree->currentItem());
        undoitem.isTopLevelItem = false;
    }
    else if(!currentItem->parent()){
        qDebug() << "No parent found";
        undoitem.position = noteTree->indexOfTopLevelItem(target);
        noteTree->takeTopLevelItem(noteTree->indexOfTopLevelItem(noteTree->currentItem()));
        undoitem.isTopLevelItem = true;
    }

    undoVector.append(undoitem);

    //qDebug() << undoVector;
    qDebug() << "Deleted item";
}

void TreeNotes::on_actionDelete_triggered()
{
    QMessageBox::StandardButton reply;

    if(appConfig.confirm_delete) reply = QMessageBox::question(this, "Delete Item", "Are you sure you want to delete the current item?\nAll the children will be removed as well.", QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes || !appConfig.confirm_delete){
        Delete((TreeWidgetItem*)noteTree->currentItem());
    }
}

void TreeNotes::MoveUp(TreeWidgetItem *item){
    if(!noteTree->currentItem()) return;

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
    if(!noteTree->currentItem()) return;

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
    IconSelectorDialog *isd = new IconSelectorDialog(this, iconVector, ((TreeWidgetItem*)noteTree->currentItem())->iconVectorIndex);

    isd->setFont(this->font());
    isd->copyFrom(noteTree);
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
        noteTree->setCurrentItem(NULL);
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
    iconVector.append(QIcon(":/Resources/Icons/star.png"));
}

void TreeNotes::InitShortcuts(){
    QShortcut *jumpToMessageBox = new QShortcut(QKeySequence(SHORTCUT_JUMP_MSG), this);
    connect(jumpToMessageBox, &QShortcut::activated, this, [&](){ui->messageEdit->setFocus();});

    QShortcut *jumpToTitleBox = new QShortcut(QKeySequence(SHORTCUT_JUMP_TITLE), this);
    connect(jumpToTitleBox, &QShortcut::activated, this, [&](){ui->titleEdit->setFocus();});

    QShortcut *jumpToTree = new QShortcut(QKeySequence(SHORTCUT_JUMP_TREE), this);
    connect(jumpToTree, &QShortcut::activated, this, [&](){
        if(!noteTree->currentItem()){
            if(noteTree->topLevelItem(0)){
                noteTree->setCurrentItem(noteTree->topLevelItem(0));
            }
        }
        noteTree->setFocus();
    });
}

void TreeNotes::InitStatusLabels(){
    qDebug() << "Init status labels";

    QFrame *line;
    line = new QFrame(ui->statusbar);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Plain);
    line->resize(this->width(), ui->statusbar->height());

    noteCntLabel = new QLabel(ui->statusbar);
    childrenCntLabel = new QLabel(ui->statusbar);
    dateTimeLabel = new QLabel(ui->statusbar);
    lineCountLabel = new QLabel(ui->statusbar);
    lengthLabel = new QLabel(ui->statusbar);
    currentPositionLabel = new QLabel(ui->statusbar);

    ui->statusbar->addWidget(noteCntLabel, 1);
    ui->statusbar->addWidget(line, 1);
    ui->statusbar->addWidget(childrenCntLabel, 1);
    ui->statusbar->addWidget(line, 1);
    ui->statusbar->addWidget(lineCountLabel, 1);
    ui->statusbar->addWidget(line, 1);
    ui->statusbar->addWidget(lengthLabel, 1);
    ui->statusbar->addWidget(line, 3);
    ui->statusbar->addWidget(currentPositionLabel, 1);
    ui->statusbar->addWidget(line, 20);
    ui->statusbar->addWidget(dateTimeLabel, 1);


    qDebug() << "Init status labels finished";
}

void TreeNotes::RefreshLabels(){
    noteCntLabel->setText(tr("Notes : %1").arg(QString::number(noteTree->noteCount())));
    if(noteTree->currentItem()){
        childrenCntLabel->setText(tr("Children: %1").arg(QString::number(noteTree->currentItem()->childCount())));
        dateTimeLabel->setText(tr("Last edited: %1").arg(((TreeWidgetItem*)noteTree->currentItem())->lastEdited.toString()));
    }
    else{
        childrenCntLabel->setText(tr("Children: %1").arg("0"));
        dateTimeLabel->setText(tr("Last edited: %1").arg(""));
    }
    if(!ui->messageEdit->textCursor().selectedText().isEmpty()){
        lengthLabel->setText(tr("Length (Sel): %1").arg(QString::number(ui->messageEdit->textCursor().selectedText().length())));
    }
    else{
        lengthLabel->setText(tr("Length: %1").arg(QString::number(ui->messageEdit->toPlainText().length())));
    }
    currentPositionLabel->setText(tr("ln: %1").arg(QString::number(ui->messageEdit->currentLine())) + tr(", col: %1").arg(QString::number(ui->messageEdit->currentColumn())));
    lineCountLabel->setText(tr("Lines: %1").arg(QString::number(ui->messageEdit->lineCount())));
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
    {
        /*
        item->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        noteTree->editItem(item);
        */
        ui->titleEdit->setFocus();
    }
}


void TreeNotes::on_actionQt_triggered()
{
    QMessageBox::aboutQt(this);
}


void TreeNotes::on_actionSet_Font_Text_Editors_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, ui->messageEdit->font(), this, tr("Select a Text Editor Font"));
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

void TreeNotes::on_actionImport_Text_File_triggered()
{
    QStringList paths = QFileDialog::getOpenFileNames(this, tr("Import Text Files"));
    foreach (QString path, paths) {
        QFile file(path);
        QFileInfo fi(file);
        if(!file.exists()) continue;
        QFuture<QString> future = QtConcurrent::run([&]() -> QString{
            file.open(QIODevice::ReadOnly);
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            return stream.readAll();
        });

        while(!future.isFinished() && future.isRunning()){
            qApp->processEvents();
        }

        if(future.result().length() > 10e+5){
            QMessageBox::warning(this, tr("Error"), tr("File too big: ") + path);
            continue;
        }

        AddNote((TreeWidgetItem*)noteTree->currentItem(), fi.fileName(), future.result() );

        file.close();
    }
}


void TreeNotes::on_actionExport_Text_File_triggered()
{
    if(!noteTree->currentItem()) return;
    QString path = QFileDialog::getSaveFileName(this, tr("Export Text File"));
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    stream << ((TreeWidgetItem*)noteTree->currentItem())->message;

    file.close();
}


void TreeNotes::on_messageEdit_textChanged()
{

}

void TreeNotes::InitMacroVector(){
    macroVec.append(QPair<QString, std::function<QString()>>("{title}", [&]() ->QString {return ui->titleEdit->text();}));
    macroVec.append(QPair<QString, std::function<QString()>>("{lastedited.second}", [&]() ->QString {return ((TreeWidgetItem*)noteTree->currentItem())->lastEdited.toString("ss");}));
    macroVec.append(QPair<QString, std::function<QString()>>("{lastedited.minute}", [&]() ->QString {return ((TreeWidgetItem*)noteTree->currentItem())->lastEdited.toString("mm");}));
    macroVec.append(QPair<QString, std::function<QString()>>("{lastedited.hour}", [&]() ->QString {return ((TreeWidgetItem*)noteTree->currentItem())->lastEdited.toString("hh");}));
    macroVec.append(QPair<QString, std::function<QString()>>("{lastedited.day}", [&]() ->QString {return ((TreeWidgetItem*)noteTree->currentItem())->lastEdited.toString("dd");}));
    macroVec.append(QPair<QString, std::function<QString()>>("{lastedited.month}", [&]() ->QString {return ((TreeWidgetItem*)noteTree->currentItem())->lastEdited.toString("MM");}));
    macroVec.append(QPair<QString, std::function<QString()>>("{lastedited.year}", [&]() ->QString {return ((TreeWidgetItem*)noteTree->currentItem())->lastEdited.toString("yyyy");}));
    macroVec.append(QPair<QString, std::function<QString()>>("{lastedited}", [&]() ->QString {return ((TreeWidgetItem*)noteTree->currentItem())->lastEdited.toString();}));
    macroVec.append(QPair<QString, std::function<QString()>>("{datetime.second}", [&]() ->QString {return QDateTime::currentDateTime().toString("ss");}));
    macroVec.append(QPair<QString, std::function<QString()>>("{datetime.minute}", [&]() ->QString {return QDateTime::currentDateTime().toString("mm");}));
    macroVec.append(QPair<QString, std::function<QString()>>("{datetime.hour}", [&]() ->QString {return QDateTime::currentDateTime().toString("hh");}));
    macroVec.append(QPair<QString, std::function<QString()>>("{datetime.day}", [&]() ->QString {return QDateTime::currentDateTime().toString("dd");}));
    macroVec.append(QPair<QString, std::function<QString()>>("{datetime.month}", [&]() ->QString {return QDateTime::currentDateTime().toString("MM");}));
    macroVec.append(QPair<QString, std::function<QString()>>("{datetime.year}", [&]() ->QString {return QDateTime::currentDateTime().toString("yyyy");}));
    macroVec.append(QPair<QString, std::function<QString()>>("{datetime}", [&]() ->QString {return QDateTime::currentDateTime().toString();}));
    macroVec.append(QPair<QString, std::function<QString()>>("{parent.message}", [&]() ->QString {if(noteTree->currentItem()->parent()){return ((TreeWidgetItem*)noteTree->currentItem()->parent())->message;}return "";}));
    macroVec.append(QPair<QString, std::function<QString()>>("{parent.title}", [&]() ->QString {if(noteTree->currentItem()->parent()){return ((TreeWidgetItem*)noteTree->currentItem()->parent())->text(0);}return "";}));
    macroVec.append(QPair<QString, std::function<QString()>>("{yes}", []() ->QString {return "✔";}));
    macroVec.append(QPair<QString, std::function<QString()>>("{no}", []() ->QString {return "✖";}));
    macroVec.append(QPair<QString, std::function<QString()>>("{lambda}", []() ->QString {return "λ";}));
    //macroVec.append(QPair<QString, std::function<QString()>>("{title}", [&]() ->QString {return ui->titleEdit->text();}));
}

void TreeNotes::on_actionMacros_triggered()
{
    MacroHelp *mh = new MacroHelp(this, macroVec);
    mh->show();
    mh->setStyleSheet(this->styleSheet());
    mh->setFont(this->font());
    mh->listWidget()->setStyleSheet(mh->listWidget()->styleSheet() + ui->messageEdit->styleSheet());
    mh->plainTextEdit()->setStyleSheet(mh->plainTextEdit()->styleSheet() +  ui->messageEdit->styleSheet());
}


void TreeNotes::on_actionUndo_Delete_triggered()
{
    start:
    if(undoVector.isEmpty()){
        qDebug() << "No undos to do";
        return;
    }
    if(!undoVector.last().item){
        undoVector.removeLast();
        goto start;
    }
    else if(!undoVector.last().parent && !undoVector.last().isTopLevelItem){
        undoVector.removeLast();
        goto start;
    }
    else{
        UndoItem undoitem = undoVector.last();
        if(undoitem.isTopLevelItem){
            noteTree->insertTopLevelItem(undoitem.position, undoitem.item);
        }
        else{
            undoitem.parent->insertChild(undoitem.position, undoitem.item);
        }
        undoVector.removeLast();
    }
}


void TreeNotes::on_actionSettings_triggered()
{
    SettingsDialog *sd = new SettingsDialog(this);
    sd->setAppConfig(appConfig);
    sd->setFont(this->font());

    sd->show();

    if(sd->exec()){
        this->appConfig = sd->appConfig();
        ReadAppConfig(this->appConfig);
        saveQSettings();
    }
}

void TreeNotes::CleanBackups(int max, QString backupsDir){
    qDebug() << "Cleaning backups, Backups dir:" << backupsDir << " Max backups:" << max;

    QDir backups(backupsDir);
    if(!backups.exists()){
        qDebug() << "Backup directory doesn't exist";
        return;
    }

    //backups.setNameFilters({"*.xml"});
    QFileInfo saveFileInfo(saveFileName);
    //"save.xml" for example gets saved as save.xml{datetime}.xml
    backups.setNameFilters({saveFileInfo.baseName() + "*"});
    QStringList entries = backups.entryList(QDir::Files, QDir::Time);

    if(max > entries.length()){
        max = entries.length();
    }

    QStringList removeList;

    for(int i = max; i < entries.count(); i++){
        removeList.append(entries[i]);
    }

    qDebug() << removeList;

    qDebug() << entries;
    qDebug() << "entrieslength" << entries.length();
    qDebug() << "removelistlength" << removeList.length();

    foreach (QString fileToRemove, removeList) {
        backups.remove(backups.absolutePath() + "/" + fileToRemove);
    }
}

void TreeNotes::on_actionStar_Unstar_triggered()
{
    if(!noteTree->currentItem()) return;
    noteTree->setStar((TreeWidgetItem*)noteTree->currentItem(), !((TreeWidgetItem*)noteTree->currentItem())->starred);
    noteTree->setCurrentItem(noteTree->currentItem());
}

void TreeNotes::on_actionExpand_All_triggered()
{
    noteTree->expandChildren((TreeWidgetItem*)noteTree->currentItem());
}

void TreeNotes::on_actionCollapse_All_triggered()
{
    noteTree->collapseChildren((TreeWidgetItem*)noteTree->currentItem());
}

void TreeNotes::on_actionRead_Only_toggled(bool arg1)
{
    ui->titleEdit->setReadOnly(arg1);
    ui->messageEdit->setReadOnly(arg1);
    ui->messageEdit->highlightCurrentLine();
    if(noteTree->currentItem()) ((TreeWidgetItem*)noteTree->currentItem())->readOnly = arg1;
}


void TreeNotes::on_titleEdit_textChanged(const QString &arg1)
{
    if(!noteTree->currentItem()) return;
    noteTree->currentItem()->setText(0, arg1);
}

void TreeNotes::ApplyMacroVector(){
    for(int i = 0; i < macroVec.count(); i++){
        ui->messageEdit->setPlainText(ui->messageEdit->toPlainText().replace(macroVec.at(i).first, macroVec.at(i).second()));
    }
}


void TreeNotes::on_actionExport_PDF_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Export PDF"));
    if(((QFileInfo)filename).suffix().isEmpty()) filename += ".pdf";
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName(filename);
    ui->messageEdit->document()->print(&printer);
}

void TreeNotes::on_actionSearch_In_Current_Note_triggered()
{
    ui->messageEdit->search();
}


void TreeNotes::on_actionReplace_All_In_Current_Note_triggered()
{
    ui->messageEdit->replaceAll();
}


void TreeNotes::on_actionSearch_In_All_Notes_triggered()
{
    SearchInAllNotesDialog *siand = new SearchInAllNotesDialog(this, ui->treeWidget);
    siand->show();
}


void TreeNotes::on_actionHide_Show_Toolbar_triggered()
{
    ui->ToolBar->setHidden(!ui->ToolBar->isHidden());
}

void TreeNotes::on_actionFullscreen_On_Off_triggered()
{
    if(this->isFullScreen()){
        this->showNormal();
    }
    else{
        this->showFullScreen();
    }
}


void TreeNotes::on_actionHide_Show_Title_2_triggered()
{
    ui->titleEdit->setHidden(!ui->titleEdit->isHidden());
}


void TreeNotes::on_actionClone_triggered()
{
    if(!noteTree->currentItem()) return;
    TreeWidgetItem *item = (TreeWidgetItem*)noteTree->currentItem();
    if(item->parent()){
        item->parent()->addChild(item->clone());
        return;
    }
    noteTree->addTopLevelItem(((TreeWidgetItem*)item)->clone());
}

