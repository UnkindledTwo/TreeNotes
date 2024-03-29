QT += core gui xml concurrent printsupport network winextras

RC_ICONS = Resources/Icon.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
#greaterThan(QT_MAJOR_VERSION, 5): error("Must be compiled with Qt 5")

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Dialogs/iconselectordialog.cpp \
    Dialogs/macrohelp.cpp \
    Dialogs/macromenu.cpp \
    Dialogs/newnotedialog.cpp \
    Dialogs/searchdialog.cpp \
    Dialogs/searchinallnotesdialog.cpp \
    Dialogs/searchinallnotesresultdialog.cpp \
    Dialogs/searchresultdialog.cpp \
    Dialogs/settingsdialog.cpp \
    Dialogs/tageditordialog.cpp \
    Dialogs/taginfodialog.cpp \
    Dialogs/tagresultsdialog.cpp \
    globals.cpp \
    main.cpp \
    plaintextedit.cpp \
    saver.cpp \
    scratchpad.cpp \
    syntaxhighlighter.cpp \
    treenotes.cpp \
    treewidget.cpp \
    treewidgetitem.cpp \
    undoitem.cpp

HEADERS += \
    Dialogs/iconselectordialog.h \
    Dialogs/macrohelp.h \
    Dialogs/macromenu.h \
    Dialogs/newnotedialog.h \
    Dialogs/searchdialog.h \
    Dialogs/searchinallnotesdialog.h \
    Dialogs/searchinallnotesresultdialog.h \
    Dialogs/searchresultdialog.h \
    Dialogs/settingsdialog.h \
    Dialogs/tageditordialog.h \
    Dialogs/taginfodialog.h \
    Dialogs/tagresultsdialog.h \
    appconfig.h \
    globals.h \
    highlightitem.h \
    plaintextedit.h \
    saver.h \
    scratchpad.h \
    syntaxhighlighter.h \
    treenotes.h \
    treewidget.h \
    treewidgetitem.h \
    undoitem.h

FORMS += \
    Dialogs/iconselectordialog.ui \
    Dialogs/macrohelp.ui \
    Dialogs/macromenu.ui \
    Dialogs/newnotedialog.ui \
    Dialogs/searchdialog.ui \
    Dialogs/searchinallnotesdialog.ui \
    Dialogs/searchinallnotesresultdialog.ui \
    Dialogs/searchresultdialog.ui \
    Dialogs/settingsdialog.ui \
    Dialogs/tageditordialog.ui \
    Dialogs/taginfodialog.ui \
    Dialogs/tagresultsdialog.ui \
    plaintextedit.ui \
    scratchpad.ui \
    treenotes.ui \
    treewidget.ui

TRANSLATIONS += Resources/Translations/tr.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
else: unix:!android: target.path = /usr/local/bin/
!isEmpty(target.path): INSTALLS += target

unix {
icon.path = /usr/share/icons/hicolor/32x32/apps/
icon.files = $$PWD/Resources/TreeNotes.png

desktopfile.path = /usr/share/applications
desktopfile.files = $$PWD/TreeNotes.desktop
l
INSTALLS += desktopfile icon
}

RESOURCES += \
    Resources.qrc\
    Resources/BreezeStyleSheets/custom.qrc \
    Translations.qrc
