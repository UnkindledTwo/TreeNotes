#include "treenotes.h"

#include "ui_treenotes.h"

TreeNotes::TreeNotes(QWidget *parent, QString saveFileName) : QMainWindow(parent), ui(new Ui::TreeNotes)
{
    qDebug() << "--------------------";
    qDebug() << "Initializing window";
    this->setWindowTitle(WINDOW_TITLE_DEFAULT);
    this->saveFileName = saveFileName;

    InitIconVector();
    InitMacroVector();

    ui->setupUi(this);

    tagsList = new QListWidget(this);
    connect(tagsList, &QListWidget::itemDoubleClicked, this, &TreeNotes::on_tag_doubleClicked);

    noteTree = ui->treeWidget;
    noteTree->clear();

    //0 = Select, 1 = Remove, 2 = Expand/Collapse, 3 = Edit Tags, 4 = Star/Unstar
    //connect(noteTree, &TreeWidget::middleClicked, this, &TreeNotes::on_actionChange_Tag_triggered);
    connect(noteTree, &TreeWidget::middleClicked, this, [&](){
        switch (Globals::appConfig.middle_click_tree_item_action) {
        case 0:
            //Do nothing
            break;
        case 1:
            on_actionDelete_triggered();
            break;
        case 2:
            noteTree->currentItem()->setExpanded(!noteTree->currentItem()->isExpanded());
            break;
        case 3:
            on_actionChange_Tag_triggered();
            break;
        case 4:
            on_actionStar_Unstar_triggered();
            break;
        default:
            break;
        }
    });

    // Construct UI
    {
        // Third portion of the UI. Tags label and tags list
        QVBoxLayout *vbox = new QVBoxLayout();
        vbox->setMargin(0);
        vbox->setSpacing(2);
        QLabel *l1 = new QLabel(this);
        l1->setText(tr("Tags"));
        vbox->addWidget(l1);
        vbox->addWidget(tagsList);
        QWidget *wrap = new QWidget();
        wrap->setLayout(vbox);

        // Initialize the splitter
        splitter = new QSplitter();
        splitter->addWidget(noteTree);

        QWidget *wrapper = new QWidget();
        ui->verticalLayout->setMargin(0);
        ui->verticalLayout->setParent(NULL);
        wrapper->setLayout(ui->verticalLayout);
        splitter->addWidget(wrapper);

        // splitter->addWidget(tagsList);
        splitter->addWidget(wrap);

        splitter->setCollapsible(0, false);
        splitter->setCollapsible(1, false);
        splitter->setCollapsible(2, false);

        splitter->setSizes({2, 3, 1});

        QGridLayout *grid = new QGridLayout();
        grid->addWidget(splitter);
        QWidget *central = new QWidget();
        central->setLayout(grid);
        this->setCentralWidget(central);
    }

    // Auto timer for refreshing label(s)
    QTimer *timer = new QTimer(this);
    timer->setInterval(50);
    connect(timer, &QTimer::timeout, this, [&]() { RefreshLabels(); });
    timer->start();

    // Set borders of noteTree, messageEdit and titleEdit to the accent color of Windows
#ifdef Q_OS_WIN
    QString styleSheet = "border: 1.750px solid " + QtWin::colorizationColor().light().name() + ";";
    styleSheet += "border-radius: 1px;";
    styleSheet += "padding: 3px;";
    ui->treeWidget->setStyleSheet("QTreeWidget{" + styleSheet + "}");
    styleSheet += "selection-color: white;";
    styleSheet += "selection-background-color:" + QtWin::colorizationColor().name() + ";";
    ui->titleEdit->setStyleSheet("QLineEdit{" + styleSheet + "}");
    ui->messageEdit->setStyleSheet("QPlainTextEdit{" + styleSheet + "}");
    tagsList->setStyleSheet("QListWidget{" + styleSheet + "}");
#else
    QString styleSheet = "border: 1.750px solid black;";
    styleSheet += "border-radius: 1px;";
    styleSheet += "padding: 3px;";
    // ui->treeWidget->setStyleSheet("QTreeWidget{" + styleSheet + "}");
    // ui->titleEdit->setStyleSheet("QLineEdit{" + styleSheet + "}");
    // ui->messageEdit->setStyleSheet("QPlainTextEdit{" + styleSheet + "}");
#endif

    ReadQSettings();
    ReadAppConfig(Globals::appConfig);

    // Connect save and load from disk actions to an existing slot
    connect(ui->actionSave_To_Disk, &QAction::triggered, this, &TreeNotes::SaveToFile);
    connect(ui->actionLoad_From_Disk, &QAction::triggered, this, &TreeNotes::ReadFromFile);

    // Context menu event
    noteTree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(noteTree, &QTreeWidget::customContextMenuRequested, this, &TreeNotes::ShowContextMenu);

    InitShortcuts();
    InitStatusLabels();
    ReadFromFile();
    ReadQSettings();
    ReadAppConfig(Globals::appConfig);

    ui->messageEdit->setZoomingEnabled(true);
    connect(ui->messageEdit, &PlainTextEdit::zoomChanged, this, [&]() { ui->titleEdit->setFont(ui->messageEdit->font()); });

    // Make sure the noteTree's column sizes are accurate
    noteTree->expandAll();
    noteTree->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    noteTree->resizeColumnToContents(1);
    noteTree->collapseAll();

    on_treeWidget_currentItemChanged(NULL, NULL);


    latestVersion();
    qDebug() << "Initilization of the main window is finished";
}

TreeNotes::~TreeNotes() { delete ui; }

QString TreeNotes::latestVersion()
{
    QUrl url = QUrl("https://api.github.com/repos/UnkindledTwo/TreeNotes/releases/latest");
    QNetworkAccessManager *mgr = new QNetworkAccessManager(this);
    QEventLoop loop;
    QNetworkReply *reply = mgr->get(QNetworkRequest(url));
    connect(mgr, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QJsonDocument doc = QJsonDocument::fromJson(((QString)reply->readAll()).toUtf8());
    QJsonObject obj = doc.object();
    qDebug() << obj["tag_name"].toString();
    return obj["tag_name"].toString();
}

void TreeNotes::updateWindowTitleAndIcon()
{
    const int smallIconSize = 20;
    QPixmap icon(":/Resources/Icon.png");

    QString title = (hasUnsavedChanges()) ? "* " : "";
    if (noteTree->currentItem() && !noteTree->currentItem()->text(0).isEmpty()) {
        // Update window icon
        {
            QPainter painter(&icon);
            painter.drawPixmap(QPoint(32 - smallIconSize, 32 - smallIconSize),
                               noteTree->currentItem()->icon(0).pixmap(smallIconSize, smallIconSize));
        }

        QString text = noteTree->currentItem()->text(0);
        if (text.length() > 30) {
            title += text.left(30) + "...";
        }
        else
            title += noteTree->currentItem()->text(0);
        title += " - ";
    }
    title += WINDOW_TITLE_DEFAULT;

    this->setWindowTitle(title);
    this->setWindowIcon(icon);
}

bool TreeNotes::hasUnsavedChanges()
{
    if (!noteTree->currentItem()) return false;
    if ((noteTree->currentItem())->message == ui->messageEdit->toPlainText()) {
        return false;
    }

    return true;
}

void TreeNotes::ShowContextMenu(const QPoint &pos)
{
    // Context Menu Event
    if (!noteTree->rect().contains(pos)) {
        return;
    }
    QMenu contextMenu(tr("Context Menu"), this);

    contextMenu.addAction(ui->actionAdd);
    contextMenu.addAction(ui->actionSave);
    contextMenu.addAction(ui->actionDelete);
    contextMenu.addAction(ui->actionUndo_Delete);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionMove_Up);
    contextMenu.addAction(ui->actionMove_Down);
    contextMenu.addAction(ui->actionFocus_Parent);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionSet_Icon);
    contextMenu.addAction(ui->actionStar_Unstar);
    contextMenu.addAction(ui->actionRead_Only);
    contextMenu.addAction(ui->actionHighlighting);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionCollapse_All);
    contextMenu.addAction(ui->actionExpand_All);
    contextMenu.addSeparator();
    contextMenu.addAction(ui->actionChange_Tag);
    contextMenu.addAction(ui->actionTag_Info);

    contextMenu.exec(QCursor().pos());
}

void TreeNotes::ReadQSettings()
{
    qDebug() << "Reading QSettings";

    // QSettings settings("Unkindled", "NoteTree", this);
#ifdef Q_OS_WIN
    QSettings settings("settings.ini", QSettings::IniFormat);
#else
    QSettings settings(QDir().homePath() + "/settings.ini", QSettings::IniFormat);
#endif
    // Read the saved font
    QFont loadedFont;
    if(!settings.value("Text_Editor_Font").isNull()) {
        loadedFont = qvariant_cast<QFont>(settings.value("Text_Editor_Font", this->font()));
    }
    else
        loadedFont = ui->messageEdit->font();
    ui->messageEdit->setFont(loadedFont);
    ui->titleEdit->setFont(loadedFont);

    ui->ToolBar->setHidden(settings.value("toolbar_hidden", false).toBool());
    ui->titleEdit->setHidden(settings.value("title_hidden", false).toBool());

    // Set window size
    if (settings.value("fullscreen", false).toBool()) {
        this->showFullScreen();
    }
    if (settings.value("maximized", false).toBool()) {
        this->showMaximized();
    }
    else {
        this->setGeometry(qvariant_cast<QRect>(settings.value("geometry", this->geometry())));
    }

    bool tagsHidden = settings.value("tags_hidden", false).toBool();
    qDebug() << "Tags hidden:" << tagsHidden;

    // Set splitter sizes
    if (settings.value("noteTreeHidden", false).toBool()) {
        on_actionHide_Show_Note_Tree_triggered();
    }
    else {
        ((QSplitter *)noteTree->parent())
                ->setSizes({settings.value("s1", this->width() / 3).toInt(), settings.value("s2", this->width() / 3).toInt(),
                            settings.value("s3", splitter->widget(2)->width()).toInt()});
    }

    qDebug() << tagsHidden;
    splitter->widget(2)->setHidden(tagsHidden);

    // Appconfig section, these can be editable from settings.ini or from the application
    settings.beginGroup("AppConfig");
    Globals::appConfig.notetree_icon_size = settings.value("notetree_icon_size", Globals::appConfig.notetree_icon_size).toInt();
    Globals::appConfig.notetree_select_new_items = settings.value("notetree_select_new_items", Globals::appConfig.notetree_select_new_items).toBool();
    Globals::appConfig.notetree_alternating_row_colors =
            settings.value("notetree_alternating_row_colors", Globals::appConfig.notetree_alternating_row_colors).toBool();
    Globals::appConfig.notetree_indentation_size = settings.value("notetree_indentation_size", Globals::appConfig.notetree_indentation_size).toInt();
    Globals::appConfig.opacity = settings.value("opacity", Globals::appConfig.opacity).toInt();
    Globals::appConfig.notetree_animated = settings.value("notetree_animated", Globals::appConfig.notetree_animated).toBool();
    Globals::appConfig.doubleClickToEditMessage = settings.value("doubleClickToEditMessage", Globals::appConfig.doubleClickToEditMessage).toBool();
    Globals::appConfig.layoutMargin = settings.value("layoutMargin", Globals::appConfig.layoutMargin).toInt();
    Globals::appConfig.splitter_handle_width = settings.value("splitter_handle_width", Globals::appConfig.splitter_handle_width).toInt();
    Globals::appConfig.confirm_delete = settings.value("confirm_delete", Globals::appConfig.confirm_delete).toBool();
    Globals::appConfig.line_wrapping = settings.value("line_wrapping", Globals::appConfig.line_wrapping).toBool();
    Globals::appConfig.pair_completion = settings.value("pair_completion", Globals::appConfig.pair_completion).toBool();
    Globals::appConfig.notetree_drag_drop = settings.value("notetree_drag_drop", Globals::appConfig.notetree_drag_drop).toBool();
    Globals::appConfig.maximum_backups = settings.value("maximum_backups", Globals::appConfig.maximum_backups).toInt();
    Globals::appConfig.use_native_theme = settings.value("use_native_theme", Globals::appConfig.use_native_theme).toBool();
    Globals::appConfig.dark_mode = settings.value("dark_mode", Globals::appConfig.dark_mode).toBool();
    Globals::appConfig.tab_width = settings.value("tab_width", Globals::appConfig.tab_width).toInt();
    Globals::appConfig.colorful_highlight = settings.value("colorful_highlight", Globals::appConfig.colorful_highlight).toBool();
    Globals::appConfig.show_new_note_dialog = settings.value("show_new_note_dialog", Globals::appConfig.show_new_note_dialog).toBool();
    settings.endGroup();
    ReadAppConfig(Globals::appConfig);

    qDebug() << "Reading QSettings is finished, file: " << settings.fileName();
}

void TreeNotes::SaveQSettings()
{
    qDebug() << "Saving QSettings";
    // QSettings settings("Unkindled", "NoteTree", this);
#ifdef Q_OS_WIN
    QSettings settings("settings.ini", QSettings::IniFormat);
#else
    QSettings settings(QDir().homePath() + "/settings.ini", QSettings::IniFormat);
#endif

    settings.setValue("Text_Editor_Font", ui->messageEdit->font());
    settings.setValue("maximized", this->isMaximized());
    settings.setValue("geometry", this->geometry());
    settings.setValue("noteTreeHidden", noteTree->isHidden());
    settings.setValue("s1", ((QSplitter *)noteTree->parent())->sizes().at(0));
    settings.setValue("s2", ((QSplitter *)noteTree->parent())->sizes().at(1));
    settings.setValue("s3", ((QSplitter *)noteTree->parent())->sizes().at(2));
    settings.setValue("toolbar_hidden", ui->ToolBar->isHidden());
    settings.setValue("title_hidden", ui->titleEdit->isHidden());
    qDebug() << "Tags hidden" << splitter->widget(2)->isHidden();
    settings.setValue("tags_hidden", splitter->widget(2)->isHidden());
    settings.setValue("fullscreen", this->isFullScreen());

    settings.beginGroup("AppConfig");

    settings.setValue("notetree_icon_size", Globals::appConfig.notetree_icon_size);
    settings.setValue("notetree_select_new_items", Globals::appConfig.notetree_select_new_items);
    settings.setValue("notetree_alternating_row_colors", Globals::appConfig.notetree_alternating_row_colors);
    settings.setValue("notetree_indentation_size", Globals::appConfig.notetree_indentation_size);
    settings.setValue("opacity", Globals::appConfig.opacity);
    settings.setValue("notetree_animated", Globals::appConfig.notetree_animated);
    settings.setValue("doubleClickToEditMessage", Globals::appConfig.doubleClickToEditMessage);
    settings.setValue("layoutMargin", Globals::appConfig.layoutMargin);
    settings.setValue("splitter_handle_width", Globals::appConfig.splitter_handle_width);
    settings.setValue("confirm_delete", Globals::appConfig.confirm_delete);
    settings.setValue("line_wrapping", Globals::appConfig.line_wrapping);
    settings.setValue("pair_completion", Globals::appConfig.pair_completion);
    settings.setValue("notetree_drag_drop", Globals::appConfig.notetree_drag_drop);
    settings.setValue("maximum_backups", Globals::appConfig.maximum_backups);
    settings.setValue("use_native_theme", Globals::appConfig.use_native_theme);
    settings.setValue("dark_mode", Globals::appConfig.dark_mode);
    settings.setValue("tab_width", Globals::appConfig.tab_width);
    settings.setValue("colorful_highlight", Globals::appConfig.colorful_highlight);
    settings.setValue("show_new_note_dialog", Globals::appConfig.show_new_note_dialog);
    settings.endGroup();

    qDebug() << "Saved QSettings, file: " << settings.fileName();
}

QString TreeNotes::dateTimeNoSpace()
{
    // I don't know what I was thinkink with this, but it works
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

void TreeNotes::AttemptSaveBackup()
{
    qDebug() << "AttempSaveBackup()";
#ifdef Q_OS_WIN
    QFileInfo backupFileInfo(qApp->applicationDirPath() + "/backup/" + saveFileName + dateTimeNoSpace() + ".xml");
#else
    //QFileInfo backupFileInfo(QDir().homePath() + "/TreeNotes/backup/" + saveFileName + dateTimeNoSpace() + ".xml");
    QFileInfo saveFileInfo(saveFileName);
    QFileInfo backupFileInfo(QDir().homePath() + "/TreeNotes/backup/" + saveFileInfo.fileName() + dateTimeNoSpace() + ".xml");
#endif
    QDir backupDir(backupFileInfo.absoluteDir().path());
    qDebug() << backupFileInfo.absoluteDir().path();
    if (!backupDir.exists()) {
        qDebug() << backupDir.absolutePath();
        backupDir.mkpath(backupDir.absolutePath());
    }

    // Move the current save file to the backups directory and rename it to save{date}.xml
#ifdef Q_OS_WIN
    backupDir.rename(qApp->applicationDirPath() + "/" + saveFileName, backupFileInfo.absoluteFilePath());
#else
    backupDir.rename(saveFileName, backupFileInfo.absoluteFilePath());
#endif

    // If the save can't be moved
    if (QFile(qApp->applicationDirPath() + "/save.xml").exists()) qCritical().noquote() << "\n!!!Backup Failed!!!\n";

    CleanBackups(Globals::appConfig.maximum_backups, backupDir.absolutePath());
    qDebug() << "AttempSaveBackup() finished";
}

void TreeNotes::ReadAppConfig(app_config appConfig)
{
    ui->messageEdit->syntaxHighlighter->setColorfulHighlighting(appConfig.colorful_highlight);
    noteTree->setIconSize(QSize(appConfig.notetree_icon_size, appConfig.notetree_icon_size));
    // Fix icon size
    noteTree->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    noteTree->resizeColumnToContents(1);

    noteTree->setIndentation(appConfig.notetree_indentation_size);
    noteTree->setAlternatingRowColors(appConfig.notetree_alternating_row_colors);
    this->setWindowOpacity(qreal(appConfig.opacity) / 100);
    noteTree->setAnimated(appConfig.notetree_animated);
    doubleClickToEditMessage = appConfig.doubleClickToEditMessage;
    centralWidget()->layout()->setMargin(appConfig.layoutMargin);
    splitter->setHandleWidth(appConfig.splitter_handle_width);
    if (appConfig.line_wrapping) {
        ui->messageEdit->setLineWrapMode(PlainTextEdit::WidgetWidth);
    }
    else {
        ui->messageEdit->setLineWrapMode(PlainTextEdit::NoWrap);
    }
    ui->messageEdit->setPairCompletion(appConfig.pair_completion);
    noteTree->setDragDrop(appConfig.notetree_drag_drop);
    if (appConfig.use_native_theme) {
        qApp->setStyleSheet("");
#ifdef Q_OS_LINUX
        qApp->setStyle(QStyleFactory::create("gtk2"));
#endif
        goto nativeTheme;
    }
    if (appConfig.dark_mode) {
        QFile file(":/dark-green/stylesheet.qss");
        file.open(QFile::ReadOnly | QFile::Text);
        QTextStream stream(&file);
        qApp->setStyleSheet(stream.readAll());
    }
    else {
        QFile file(":/light/stylesheet.qss");
        file.open(QFile::ReadOnly | QFile::Text);
        QTextStream stream(&file);
        qApp->setStyleSheet(stream.readAll());
    }

    qDebug() << appConfig.tab_width;
    ui->messageEdit->setTabStopWidth(appConfig.tab_width);

nativeTheme:

    qDebug() << "App config read finished";
}

void TreeNotes::closeEvent(QCloseEvent *e)
{
    qDebug().noquote() << "\n\n--------------------\nClose Event" << e;
    // Save the selected item
    if (noteTree->currentItem()) {
        Save(noteTree->currentItem());
    }

    AttemptSaveBackup();
    SaveToFile();
    SaveQSettings();
    qDebug() << "Closed window";
    qDebug() << "--------------------";
}

void TreeNotes::ReadFromFile()
{
    Saver s(this->noteTree, Globals::iconVector);
    s.saveFileName = this->saveFileName;
    s.ReadFromFile();
}

void TreeNotes::SaveToFile()
{
    Saver s(this->noteTree, Globals::iconVector);
    s.saveFileName = this->saveFileName;
    s.SaveToFile();
}

void TreeNotes::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Return) {
        if (noteTree->currentItem() && noteTree->hasFocus()) {
            noteTree->currentItem()->setExpanded(!noteTree->currentItem()->isExpanded());
        }
    }

    if (e->key() == Qt::Key_Delete) {
        if (noteTree->currentItem() && noteTree->hasFocus()) {
            on_actionDelete_triggered();
        }
    }
}

void TreeNotes::Save(TreeWidgetItem *target)
{
    if (ui->messageEdit->toPlainText() == target->message && ui->titleEdit->text() == target->text(0)) {
        return;
    }

    ApplyMacroVector();

    target->lastEdited = QDateTime::currentDateTime();

    int savedPos = ui->messageEdit->textCursor().position();

    target->message = ui->messageEdit->toPlainText();
    target->setText(0, ui->titleEdit->text());

    on_treeWidget_currentItemChanged(noteTree->currentItem(), NULL);

    if (savedPos > target->message.length()) {
        savedPos = target->message.length();
    }

    QTextCursor a = ui->messageEdit->textCursor();
    a.setPosition(savedPos);
    ui->messageEdit->setTextCursor(a);

    updateWindowTitleAndIcon();
}

void TreeNotes::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    qDebug() << "Item changed";
    if (!current) {
        tagsList->clear();
        ui->messageEdit->clear();
        ui->titleEdit->clear();
        ui->messageEdit->setEnabled(false);
        ui->titleEdit->setEnabled(false);
        tagsList->setEnabled(false);
        ui->actionHighlighting->setChecked(false);
        return;
    }
    ui->messageEdit->setEnabled(true);
    ui->titleEdit->setEnabled(true);
    tagsList->setEnabled(true);

    TreeWidgetItem *previousItem = (TreeWidgetItem *)previous;
    if (!previous || !previousItem) goto part2;
    Save(previousItem);

part2:
    TreeWidgetItem *currentItem = (TreeWidgetItem *)current;
    if (!currentItem) {
        return;
    }

    ui->messageEdit->fastSetPlainText(currentItem->message);
    ui->titleEdit->setText(currentItem->text(0));
    ui->messageEdit->setHighlighting(currentItem->highlighting);
    ui->actionHighlighting->setChecked(currentItem->highlighting);

    tagsList->clear();
    foreach (QString tag, currentItem->tags) {
        tagsList->addItem(tag);
    }

    if (currentItem->readOnly != ui->messageEdit->isReadOnly()) {
        ui->messageEdit->setReadOnly(currentItem->readOnly);
        ui->titleEdit->setReadOnly(currentItem->readOnly);
        ui->actionRead_Only->setChecked(currentItem->readOnly);
    }
}

void TreeNotes::on_actionAdd_triggered() {
    if(Globals::appConfig.show_new_note_dialog) {
        NewNoteDialog* nnd = new NewNoteDialog(ui->treeWidget, this);
        nnd->show();
    }
    else {
        noteTree->addNote(noteTree->currentItem(), tr("New Note"), "");
    }
}

void TreeNotes::on_actionSave_triggered()
{
    if (!noteTree->currentItem()) {
        showNoSelectedItemDialog();
        return;
    }
    Save(noteTree->currentItem());
}

void TreeNotes::Delete(QTreeWidgetItem *target)
{
    qDebug() << "Started deleting item";
    if (!target) {
        qDebug() << "Found no item, return";
        return;
    }

    qDebug() << "Parent of selected item: " << target->parent();
    QTreeWidgetItem *currentItem = target;

    UndoItem undoitem;
    undoitem.item = target;
    undoitem.parent = target->parent();

    if (currentItem->parent() != 0x0) {
        undoitem.position = target->parent()->indexOfChild(target);
        target->parent()->removeChild(noteTree->currentItem());
        undoitem.isTopLevelItem = false;
    }
    else if (!currentItem->parent()) {
        qDebug() << "No parent found";
        undoitem.position = noteTree->indexOfTopLevelItem(target);
        noteTree->takeTopLevelItem(noteTree->indexOfTopLevelItem(noteTree->currentItem()));
        undoitem.isTopLevelItem = true;
    }

    undoVector.append(undoitem);

    // qDebug() << undoVector;
    qDebug() << "Deleted item";
}

void TreeNotes::on_actionDelete_triggered()
{
    if (!noteTree->currentItem()) {
        showNoSelectedItemDialog();
        return;
    }

    QMessageBox::StandardButton reply;

    if (Globals::appConfig.confirm_delete)
        reply = QMessageBox::question(this, tr("Delete Item"),
                                      tr("Are you sure you want to delete the current item?\nAll the "
                                      "children will be removed as well."),
                                      QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes || !Globals::appConfig.confirm_delete) {
        Delete(noteTree->currentItem());
    }
}

void TreeNotes::MoveUp(TreeWidgetItem *item)
{
    TreeWidgetItem *i = noteTree->currentItem();
    bool expanded = i->isExpanded();
    int row = noteTree->currentIndex().row();
    if (item && item->parent() && row > 0) {
        TreeWidgetItem *parent = i->parent();
        int index = parent->indexOfChild(i);
        TreeWidgetItem *child = parent->takeChild(index);
        parent->insertChild(index - 1, child);
    }
    else if (item && row > 0) {
        int index = noteTree->indexOfTopLevelItem(i);
        TreeWidgetItem *child = noteTree->takeTopLevelItem(index);
        noteTree->insertTopLevelItem(index - 1, child);
    }

    noteTree->setCurrentItem(i);
    i->setExpanded(expanded);
}

void TreeNotes::MoveDown(TreeWidgetItem *item)
{
    TreeWidgetItem *i = noteTree->currentItem();
    bool expanded = i->isExpanded();
    int row = noteTree->currentIndex().row();
    if (item && item->parent() && row < i->parent()->childCount() - 1) {
        TreeWidgetItem *parent = i->parent();
        int index = parent->indexOfChild(i);
        TreeWidgetItem *child = parent->takeChild(index);
        parent->insertChild(index + 1, child);
    }
    else if (item && row < noteTree->topLevelItemCount() - 1) {
        int index = noteTree->indexOfTopLevelItem(i);
        TreeWidgetItem *child = noteTree->takeTopLevelItem(index);
        noteTree->insertTopLevelItem(index + 1, child);
    }

    noteTree->setCurrentItem(i);
    i->setExpanded(expanded);
}

void TreeNotes::on_actionMove_Up_triggered()
{
    if (!noteTree->currentItem()) {
        showNoSelectedItemDialog();
        return;
    }
    MoveUp(noteTree->currentItem());
}

void TreeNotes::on_actionMove_Down_triggered()
{
    if (!noteTree->currentItem()) {
        showNoSelectedItemDialog();
        return;
    }
    MoveDown(noteTree->currentItem());
}

void TreeNotes::on_actionSet_Icon_triggered()
{
    if (!noteTree->currentItem()) {
        showNoSelectedItemDialog();
        return;
    }
    IconSelectorDialog *isd = new IconSelectorDialog(this, noteTree->currentItem()->iconVectorIndex);

    isd->copyFrom(noteTree);
    if (isd->exec()) {
        noteTree->currentItem()->setIcon(0, isd->selectedIcon);
        noteTree->currentItem()->iconVectorIndex = isd->index;
        qDebug() << QString::number(isd->index);
    }
    updateWindowTitleAndIcon();
}

void TreeNotes::on_actionFocus_Parent_triggered()
{
    if (!noteTree->currentItem()) {
        showNoSelectedItemDialog();
        return;
    }
    if (noteTree->currentItem()->parent()) {
        noteTree->setCurrentItem(noteTree->currentItem()->parent());
    }
    else {
        noteTree->clearSelection();
        noteTree->setCurrentItem(NULL);
    }
}

void TreeNotes::InitIconVector()
{
    Globals::iconVector = QVector<QIcon>{QIcon(":/Resources/Icon.ico"),
            style()->standardIcon(QStyle::SP_FileIcon),
            style()->standardIcon(QStyle::SP_DirIcon),
            style()->standardIcon(QStyle::SP_MessageBoxWarning),
            style()->standardIcon(QStyle::SP_MessageBoxCritical),
            style()->standardIcon(QStyle::SP_ComputerIcon),
            style()->standardIcon(QStyle::SP_DirHomeIcon),
            style()->standardIcon(QStyle::SP_FileLinkIcon),
            style()->standardIcon(QStyle::SP_TrashIcon),
            style()->standardIcon(QStyle::SP_DialogSaveButton),
            style()->standardIcon(QStyle::SP_FileDialogNewFolder),
            style()->standardIcon(QStyle::SP_FileDialogStart),
            style()->standardIcon(QStyle::SP_DialogApplyButton),
            style()->standardIcon(QStyle::SP_DesktopIcon),
            style()->standardIcon(QStyle::SP_ArrowBack),
            QIcon(":/Resources/Icons/star.png"),
            QIcon(":/Resources/Icons/tux.png"),
            QIcon(":/Resources/Icons/windows.png")};
}

void TreeNotes::InitShortcuts()
{
#define JUMPMESSAGE ui->messageEdit->setFocus();
#define JUMPTREE                                                \
    if (!noteTree->currentItem()) {                             \
    if (noteTree->topLevelItem(0)) {                         	\
    noteTree->setCurrentItem(noteTree->topLevelItem(0)); 		\
}                                                        		\
}                                                            	\
    noteTree->setFocus();
#define JUMPTITLE ui->titleEdit->setFocus();

    QShortcut *jumpToMessageBox = new QShortcut(QKeySequence(SHORTCUT_JUMP_MSG), this);
    connect(jumpToMessageBox, &QShortcut::activated, this, [&]() {
        if (ui->messageEdit->hasFocus()) {
            JUMPTREE
        }
        else
            JUMPMESSAGE
    });

    QShortcut *jumpToTitleBox = new QShortcut(QKeySequence(SHORTCUT_JUMP_TITLE), this);
    connect(jumpToTitleBox, &QShortcut::activated, this, [&]() { JUMPTITLE });

    QShortcut *jumpToTree = new QShortcut(QKeySequence(SHORTCUT_JUMP_TREE), this);
    connect(jumpToTree, &QShortcut::activated, this, [&]() {
        if (!noteTree->hasFocus()) {
            JUMPTREE
        }
        else
            JUMPMESSAGE
    });
}

void TreeNotes::InitStatusLabels()
{
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
    readOnlyLabel = new QLabel(QString("<b>%1</b>").arg(tr("Read Only")));

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
    ui->statusbar->addWidget(readOnlyLabel, 1);
    ui->statusbar->addWidget(dateTimeLabel, 1);

    qDebug() << "Init status labels finished";
}

void TreeNotes::RefreshLabels()
{
    noteCntLabel->setText(tr("Notes : %1").arg(QString::number(noteTree->noteCount())));
    if (noteTree->currentItem()) {
        if (noteTree->currentItem()->readOnly) {
            // readOnlyLabel->setText(tr("Read Only"));
            readOnlyLabel->show();
        }
        else
            readOnlyLabel->hide();

        childrenCntLabel->setText(tr("Children: %1").arg(QString::number(noteTree->currentItem()->childCount())));
        dateTimeLabel->setText(tr("Last edited: %1").arg(noteTree->currentItem()->lastEdited.toString()));
    }
    else {
        readOnlyLabel->hide();
        childrenCntLabel->setText(tr("Children: %1").arg("0"));
        dateTimeLabel->setText(tr("Last edited: %1").arg(""));
    }
    if (!ui->messageEdit->textCursor().selectedText().isEmpty()) {
        lengthLabel->setText(tr("Length (Sel): %1").arg(QString::number(ui->messageEdit->textCursor().selectedText().length())));
    }
    else {
        lengthLabel->setText(tr("Length: %1").arg(QString::number(ui->messageEdit->toPlainText().length())));
    }
    currentPositionLabel->setText(tr("ln: %1").arg(QString::number(ui->messageEdit->currentLine())) +
                                  tr(", col: %1").arg(QString::number(ui->messageEdit->currentColumn())));
    lineCountLabel->setText(tr("Lines: %1").arg(QString::number(ui->messageEdit->blockCount())));
}

void TreeNotes::on_actionHide_Show_Note_Tree_triggered()
{
    if (noteTree->isHidden()) {
        noteTree->show();
    }
    else {
        noteTree->hide();
    }
}

void TreeNotes::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    qDebug() << "Item double clicked" << item << "column" << column;
    if (doubleClickToEditMessage) {
        /*
        item->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        noteTree->editItem(item);
        */
        ui->titleEdit->setFocus();
    }
}

void TreeNotes::on_actionQt_triggered() { QMessageBox::aboutQt(this); }

void TreeNotes::on_actionSet_Font_Text_Editors_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, ui->messageEdit->font(), this, tr("Select a Text Editor Font"));
    if (ok) {
        ui->titleEdit->setFont(font);
        ui->messageEdit->setFont(font);
    }
}

QString TreeNotes::boolToString(bool a)
{
    if (a) {
        return "true";
    }
    if (!a) {
        return "false";
    }
    return "";
}

bool TreeNotes::stringToBool(QString a) { return a == "true"; }

void TreeNotes::on_tag_doubleClicked(QListWidgetItem *item)
{
    qDebug() << "Tag double clicked";
    TagResultsDialog *trd = new TagResultsDialog(item->text(), noteTree, this);
    trd->show();
}

void TreeNotes::on_actionImport_Text_File_triggered()
{
    QStringList paths = QFileDialog::getOpenFileNames(this, tr("Import Text Files"));
    foreach (QString path, paths) {
        QFile file(path);
        QFileInfo fi(file);
        if (!file.exists()) continue;
        QFuture<QString> future = QtConcurrent::run([&]() -> QString {
            file.open(QIODevice::ReadOnly);
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            return stream.readAll();
        });

        while (!future.isFinished() && future.isRunning()) {
            qApp->processEvents();
        }

        if (future.result().length() > 10e+5) {
            QMessageBox::warning(this, tr("Error"), tr("File too big: ") + path);
            continue;
        }

        noteTree->addNote(noteTree->currentItem(), fi.fileName(), future.result());

        file.close();
    }
}

void TreeNotes::on_actionExport_Text_File_triggered()
{
    if (!noteTree->currentItem()) {
        showNoSelectedItemDialog();
        return;
    }
    QString path = QFileDialog::getSaveFileName(this, tr("Export Text File"));
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    stream << noteTree->currentItem()->message;

    file.close();
}

void TreeNotes::on_messageEdit_textChanged() { updateWindowTitleAndIcon(); }

void TreeNotes::InitMacroVector()
{
    Globals::macroVector = {
        {"{title}", [&]() -> QString { return ui->titleEdit->text(); }},
        {"{lastedited}", [&]() -> QString { return noteTree->currentItem()->lastEdited.toString(); }},
        {"{lastedited.time}", [&]() -> QString { return noteTree->currentItem()->lastEdited.time().toString(); }},
        {"{lastedited.date}", [&]() -> QString { return noteTree->currentItem()->lastEdited.date().toString(); }},
        {"{datetime}", [&]() -> QString { return QDateTime::currentDateTime().toString(); }},
        {"{date}", [&]() -> QString { return QDate::currentDate().toString(); }},
        {"{time}", [&]() -> QString { return QTime::currentTime().toString(); }},
        {"{parent.message}", [&]() -> QString {
                         if (noteTree->currentItem()->parent()) {
                             return (noteTree->currentItem()->parent())->message;
                         }
                         return "";
                     }},
        {"{parent.title}", [&]() -> QString {
                         if (noteTree->currentItem()->parent()) {
                             return (noteTree->currentItem()->parent())->text(0);
                         }
                         return "";
                     }},
        {"{yes}", []() -> QString { return "✔"; }},
        {"{no}", []() -> QString { return "✖"; }},
        {"{lambda}", []() -> QString { return "λ"; }},
        {"{cb_empty}", []() -> QString { return "☐"; }},
        {"{cb_yes}", []() -> QString { return "☑"; }},
        {"{cb_no}", []() -> QString { return "☒"; }},
        {"{star}", []() -> QString { return "★"; }},
        {"{star_hollow}", []() -> QString { return "☆"; }}
    };
}

void TreeNotes::on_actionMacros_triggered()
{
    MacroHelp *mh = new MacroHelp(this);
    mh->show();
    mh->setStyleSheet(this->styleSheet());
    mh->listWidget()->setStyleSheet(mh->listWidget()->styleSheet() + ui->messageEdit->styleSheet());
    mh->plainTextEdit()->setStyleSheet(mh->plainTextEdit()->styleSheet() + ui->messageEdit->styleSheet());
}

void TreeNotes::on_actionUndo_Delete_triggered()
{
start:
    if (undoVector.isEmpty()) {
        qDebug() << "Nothing to undo";
        QMessageBox::information(this, tr("Undo Delete"), tr("Nothing to undo"));
        return;
    }
    if (!undoVector.last().item) {
        undoVector.removeLast();
        goto start;
    }
    else if (!undoVector.last().parent && !undoVector.last().isTopLevelItem) {
        undoVector.removeLast();
        goto start;
    }
    else {
        UndoItem undoitem = undoVector.last();
        if (undoitem.isTopLevelItem) {
            noteTree->insertTopLevelItem(undoitem.position, undoitem.item);
        }
        else {
            undoitem.parent->insertChild(undoitem.position, undoitem.item);
        }
        undoVector.removeLast();
    }
}

void TreeNotes::on_actionSettings_triggered()
{
    SettingsDialog *sd = new SettingsDialog(this);
    sd->setAppConfig(Globals::appConfig);

    sd->show();

    if (sd->exec()) {
        Globals::appConfig = sd->appConfig();
        ReadAppConfig(Globals::appConfig);
        SaveQSettings();
    }
}

void TreeNotes::CleanBackups(int max, QString backupsDir)
{
    qDebug() << "Cleaning backups, Backups dir:" << backupsDir << " Max backups:" << max;

    QDir backups(backupsDir);
    if (!backups.exists()) {
        qDebug() << "Backup directory doesn't exist";
        return;
    }

    // backups.setNameFilters({"*.xml"});
    QFileInfo saveFileInfo(saveFileName);
    //"save.xml" for example gets saved as save.xml{datetime}.xml
    backups.setNameFilters({saveFileInfo.baseName() + "*"});
    QStringList entries = backups.entryList(QDir::Files, QDir::Time);

    if (max > entries.length()) {
        max = entries.length();
    }

    QStringList removeList;

    for (int i = max; i < entries.count(); i++) {
        removeList.append(entries[i]);
    }

    qDebug() << removeList;
    qDebug() << "Total backups to remove:" << removeList.length();

    foreach (QString fileToRemove, removeList) {
        backups.remove(backups.absolutePath() + "/" + fileToRemove);
    }
}

void TreeNotes::on_actionStar_Unstar_triggered()
{
    if (!noteTree->currentItem()) {
        showNoSelectedItemDialog();
        return;
    }
    noteTree->setStar(noteTree->currentItem(), !noteTree->currentItem()->isStarred());
    noteTree->setCurrentItem(noteTree->currentItem());
}

void TreeNotes::on_actionExpand_All_triggered()
{
    if (!noteTree->currentItem()) {
        showNoSelectedItemDialog();
    }
    noteTree->expandChildren(noteTree->currentItem());
}

void TreeNotes::on_actionCollapse_All_triggered()
{
    if (!noteTree->currentItem()) {
        showNoSelectedItemDialog();
    }
    noteTree->collapseChildren(noteTree->currentItem());
}

void TreeNotes::on_actionRead_Only_toggled(bool arg1)
{
    ui->titleEdit->setReadOnly(arg1);
    ui->messageEdit->setReadOnly(arg1);
    if (noteTree->currentItem()) noteTree->currentItem()->readOnly = arg1;
}

void TreeNotes::on_titleEdit_textChanged(const QString &arg1)
{
    if (!noteTree->currentItem()) return;
    noteTree->currentItem()->setText(0, arg1);

    updateWindowTitleAndIcon();
}

void TreeNotes::ApplyMacroVector()
{
    for (int i = 0; i < Globals::macroVector.count(); i++) {
        ui->messageEdit->setPlainText(ui->messageEdit->toPlainText().replace(Globals::macroVector.at(i).first, Globals::macroVector.at(i).second()));
    }
}

void TreeNotes::on_actionExport_PDF_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Export PDF"));
    if (((QFileInfo)filename).suffix().isEmpty()) filename += ".pdf";
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName(filename);
    ui->messageEdit->document()->print(&printer);
}

void TreeNotes::on_actionSearch_In_Current_Note_triggered() { ui->messageEdit->search(); }

void TreeNotes::on_actionReplace_All_In_Current_Note_triggered() { ui->messageEdit->replaceAll(); }

void TreeNotes::on_actionSearch_In_All_Notes_triggered()
{
    SearchInAllNotesDialog *siand = new SearchInAllNotesDialog(this, ui->treeWidget);
    siand->show();
}

void TreeNotes::on_actionHide_Show_Toolbar_triggered() { ui->ToolBar->setHidden(!ui->ToolBar->isHidden()); }

void TreeNotes::on_actionFullscreen_On_Off_triggered()
{
    if (this->isFullScreen()) {
        this->showNormal();
    }
    else {
        this->showFullScreen();
    }
}

void TreeNotes::on_actionHide_Show_Title_2_triggered() { ui->titleEdit->setHidden(!ui->titleEdit->isHidden()); }

void TreeNotes::on_actionClone_triggered()
{
    if (!noteTree->currentItem()) {
        showNoSelectedItemDialog();
        return;
    }

    TreeWidgetItem *item = noteTree->currentItem();
    if (item->parent()) {
        item->parent()->addChild(item->clone());
        return;
    }
    noteTree->addTopLevelItem(item->clone());
}

void TreeNotes::on_actionChange_Layout_triggered()
{
    // Swap sizes
    QList<int> sizes = splitter->sizes();
    int item1 = sizes[0];
    int item2 = sizes[1];
    sizes[1] = item1;
    sizes[0] = item2;

    // Swap items in place in splitter
    QWidget *layoutItem1 = splitter->widget(0);
    QWidget *layoutItem2 = splitter->widget(1);
    layoutItem1->setParent(NULL);
    layoutItem2->setParent(NULL);
    splitter->addWidget(layoutItem2);
    splitter->addWidget(layoutItem1);

    splitter->setSizes(sizes);
}

void TreeNotes::on_actionCheck_For_The_Latest_Version_triggered()
{
    QString v = latestVersion();
    QString version = v.right(v.length() - 1);
    qDebug() << qApp->applicationVersion();
    if(version == "") {
        //Might be an issue with the network module not being able to connect.
        QMessageBox msg(this);
        msg.setWindowTitle(tr("Check For Updates"));
        msg.setTextFormat(Qt::RichText);
        msg.setText(tr("An error occured. Please check your connection. "
                    "If you are sure you have internet connection please open an issue at "
                    "<a href='https://github.com/UnkindledTwo/TreeNotes/'>my github</a>"));
        msg.setIcon(QMessageBox::Warning);
        msg.exec();
    }
    else if (version == qApp->applicationVersion().left(3)) {
        QMessageBox::information(this, tr("Check For Updates"), tr("TreeNotes is up to date"));
    }
    else {
        QMessageBox::warning(this, tr("Check For Updates"), tr("TreeNotes is not up to date"));
    }
}

void TreeNotes::on_actionChange_Tag_triggered()
{
    if (!noteTree->currentItem()) {
        showNoSelectedItemDialog();
        return;
    }

    qDebug() << noteTree->currentItem()->tags;
    TagEditorDialog *ted = new TagEditorDialog(noteTree->currentItem(), this);
    ted->show();
    connect(ted, &TagEditorDialog::accepted, this, [&]() {
        tagsList->clear();
        foreach (QString tag, noteTree->currentItem()->tags) {
            tagsList->addItem(tag);
        }
    });
}

void TreeNotes::on_actionHide_Show_Tags_triggered()
{
    // tagsList->setHidden(!tagsList->isHidden());
    splitter->widget(2)->setHidden(!splitter->widget(2)->isHidden());
    if (!tagsList->isHidden()) {
        QList<int> sizes = splitter->sizes();
        if (sizes[2] < splitter->width() / 5) {
            sizes[2] = splitter->width() / 5;
            splitter->setSizes(sizes);
        }
    }
}

void TreeNotes::on_actionTag_Info_triggered()
{
    TagInfoDialog *tid = new TagInfoDialog(noteTree, this);
    tid->show();
}

void TreeNotes::showNoSelectedItemDialog() { QMessageBox::warning(this, tr("Error"), tr("No Note Selected")); }
void TreeNotes::on_actionNew_Scratch_Pad_triggered()
{
    bool ok;
    QString name = QInputDialog::getText(this, tr("Enter name for the new scratchpad"), tr("Name: "), QLineEdit::Normal,
                                         QString("Scratch Pad #%1").arg(ui->menuScratch_Pad->actions().count()), &ok);
    if (!ok || name.isEmpty()) return;

    createScratchPad(name);
}

void TreeNotes::createScratchPad(QString name)
{
    // Construct scratch pad
    ScratchPad *newPad = new ScratchPad();
    newPad->textEdit->setZoomingEnabled(ui->messageEdit->zoomingEnabled());
    newPad->textEdit->setFont(ui->messageEdit->font());
    newPad->setWindowFlags(newPad->windowFlags() | Qt::WindowStaysOnTopHint);
    newPad->setWindowTitle(name + " | Scratch Pad");
    scratchPads.append(newPad);
    connect(newPad, &ScratchPad::deleteRequested, this, [&](ScratchPad *pad) {
        int index = scratchPads.indexOf(pad);
        scratchPads.remove(index);
        ui->menuScratch_Pad->removeAction(ui->menuScratch_Pad->actions()[index]);
    });

    QAction *action = new QAction();
    action->setText(name);
    connect(action, &QAction::triggered, this, &TreeNotes::on_scratchpad_action_triggered);
    ui->menuScratch_Pad->insertAction((QAction *)sender(), action);

    newPad->show();
}

void TreeNotes::openScratchPad(int index) { scratchPads[index]->show(); }

void TreeNotes::on_scratchpad_action_triggered() { openScratchPad(ui->menuScratch_Pad->actions().indexOf((QAction *)sender())); }

void TreeNotes::on_actionHighlighting_triggered(bool checked)
{
    if(!noteTree->currentItem()) return;
    noteTree->currentItem()->highlighting = checked;
    ui->messageEdit->setHighlighting(checked);
}

void TreeNotes::on_actionMacro_Menu_triggered()
{
    ui->messageEdit->showMacroMenu();
}

