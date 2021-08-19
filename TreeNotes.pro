QT += core gui xml concurrent printsupport

win32
{
    QT += winextras
}


RC_ICONS = Resources/Icon.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 5): error("Must be compiled with Qt 5")

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Dialogs/iconselectordialog.cpp \
    Dialogs/macrohelp.cpp \
    Dialogs/searchdialog.cpp \
    Dialogs/searchinallnotesdialog.cpp \
    Dialogs/searchinallnotesresultdialog.cpp \
    Dialogs/searchresultdialog.cpp \
    Dialogs/settingsdialog.cpp \
    highlightitem.cpp \
    main.cpp \
    plaintextedit.cpp \
    saver.cpp \
    treenotes.cpp \
    treewidget.cpp \
    treewidgetitem.cpp \
    undoitem.cpp

HEADERS += \
    Dialogs/iconselectordialog.h \
    Dialogs/macrohelp.h \
    Dialogs/searchdialog.h \
    Dialogs/searchinallnotesdialog.h \
    Dialogs/searchinallnotesresultdialog.h \
    Dialogs/searchresultdialog.h \
    Dialogs/settingsdialog.h \
    appconfig.h \
    highlightitem.h \
    plaintextedit.h \
    saver.h \
    treenotes.h \
    treewidget.h \
    treewidgetitem.h \
    undoitem.h

FORMS += \
    Dialogs/iconselectordialog.ui \
    Dialogs/macrohelp.ui \
    Dialogs/searchdialog.ui \
    Dialogs/searchinallnotesdialog.ui \
    Dialogs/searchinallnotesresultdialog.ui \
    Dialogs/searchresultdialog.ui \
    Dialogs/settingsdialog.ui \
    plaintextedit.ui \
    treenotes.ui \
    treewidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc\
    Resources/BreezeStyleSheets/custom.qrc


