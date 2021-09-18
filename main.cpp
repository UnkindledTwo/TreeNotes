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
    qDebug() << "Locale:" << QLocale().name().left(2);
    if(t.load(":/Resources/Translations/" + QLocale().name().left(2) + ".qm")){
        qDebug() << "Translator loaded successfully";
    }
    //a.installTranslator(&t);

    //Load another save file if specified
        //The default save name is save.xml
    QString saveFileName = "save.xml";
    if(argc > 1) {
        qDebug() << "Save File Name Specified:" << QString(argv[1]);
        saveFileName = QString(argv[1]);

        //Check if file name if valid
        bool fileNameIsValid = true;
        #ifdef Q_OS_WIN
        //For windows, check this link
        //https://docs.microsoft.com/en-us/windows/win32/fileio/naming-a-file

        QString illegalChars[7] = {"<", ">", ":", "\"", "|", "?", "*"};
        for(int i = 0; i < 7;i++) {
            if(saveFileName.toLower().indexOf(illegalChars[i].toLower()) != -1) {
                fileNameIsValid = false;
                qDebug() << "Illegal character" << illegalChars[i] << "detected.";

                break;
            }
        }

        QFileInfo saveFileInfo(saveFileName);
        QVector<QString> illegalNames = {"CON", "PRN", "AUX", "NUL"};
        //Add COM0 through COM9
        for(int i = 0; i < 10; i++) {
            illegalNames.append("COM" + QString::number(i));
        }
        //Add LPT0 through LPT9
        for(int i = 0; i < 10; i++) {
            illegalNames.append("LPT" + QString::number(i));
        }

        for(int i = 0; i < illegalNames.length(); i++) {
            if(saveFileInfo.completeBaseName().toLower() == illegalNames[i].toLower()) {
                fileNameIsValid = false;
                qDebug() << "Illegal name" << illegalNames[i] << "detected";

                break;
            }
        }
        #else
        /*
         * For Linux and MacOS I'm not sure.
            Under Linux and other Unix-related systems,
            there are only two characters that cannot appear in the name of a file or directory,
            and those are NUL '\0' and slash '/'.
            (NUL is taken care by Qt I believe)
            The slash, of course,
            can appear in a path name,
            separating directory components.
            Rumour1 has it that Steven Bourne (of 'shell' fame) had a directory containing 254 files,
            one for every single letter (character code) that can appear in a file name
            (excluding /, '\0'; the name .
            was the current directory, of course).
            It was used to test the Bourne shell and routinely wrought havoc on unwary programs such as backup programs.
         */
        #endif

        if(!fileNameIsValid) {
            qDebug() << "File Name Specified Is Not Valid";
            return -1;
        }
    }
    TreeNotes w(NULL, saveFileName);

    //Splash screen
    QSplashScreen ss(QPixmap(":/Resources/Icon.png").scaled(w.size()));
    ss.move(w.pos());
    ss.show();
    w.show();
    ss.finish(&w);

    qDebug() << QLocale().uiLanguages();

    return a.exec();
}
