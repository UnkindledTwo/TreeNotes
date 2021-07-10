#include "treenotes.h"

#include <QApplication>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TreeNotes w;
    QSplashScreen ss(QPixmap(":/Resources/Icon.png").scaled(w.size()));
    ss.move(w.pos());
    ss.show();
    w.show();
    ss.finish(&w);
    return a.exec();
}
