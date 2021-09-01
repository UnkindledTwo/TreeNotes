#include "treenotes.h"

#include <QApplication>
#include <QSplashScreen>
#include <QDateTime>
#include <QDebug>
#include <QTranslator>
#include <QLocale>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator t;
    qDebug() << "Locale: " << QLocale().name().left(2);
    if(t.load(":/Resources/Translations/" + QLocale().name().left(2) + ".qm")){
        qDebug() << "Translator";
    }
    a.installTranslator(&t);

    TreeNotes w;
    QSplashScreen ss(QPixmap(":/Resources/Icon.png").scaled(w.size()));

    ss.move(w.pos());
    ss.show();
    w.show();
    ss.finish(&w);

    qDebug() << QLocale().uiLanguages();

    return a.exec();
}
