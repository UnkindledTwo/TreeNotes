#ifndef SAVER_H
#define SAVER_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QtXml>
#include <QtConcurrent>
#include "treewidget.h"

class Saver
{
public:
    QString saveFileName;
    Saver(TreeWidget *noteTree, QVector<QIcon> iconVector);
    void SaveToFile();
    void ReadFromFile();
private:
    void AddChildren(QDomDocument *doc ,QDomElement *elem, QTreeWidgetItem *parent);
    void ReadChildren(QDomDocument *doc, QDomNode node, TreeWidgetItem *parent);

    QVector<QIcon> iconVector;
    TreeWidget *noteTree;
};

#endif // SAVER_H
