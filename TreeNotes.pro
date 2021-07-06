QT       += core gui xml winextras

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
    Dialogs/settingsdialog.cpp \
    main.cpp \
    plaintextedit.cpp \
    treenotes.cpp \
    treewidgetitem.cpp \
    undoitem.cpp

HEADERS += \
    Dialogs/iconselectordialog.h \
    Dialogs/macrohelp.h \
    Dialogs/settingsdialog.h \
    appconfig.h \
    plaintextedit.h \
    treenotes.h \
    treewidgetitem.h \
    undoitem.h

FORMS += \
    Dialogs/iconselectordialog.ui \
    Dialogs/macrohelp.ui \
    Dialogs/settingsdialog.ui \
    plaintextedit.ui \
    treenotes.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc
