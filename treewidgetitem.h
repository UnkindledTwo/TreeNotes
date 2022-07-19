#ifndef TREEWIDGETITEM_H
#define TREEWIDGETITEM_H

#include <QDateTime>
#include <QDebug>
#include <QObject>
#include <QTreeWidget>
#include <QWidget>

class TreeWidgetItem : public QTreeWidgetItem {
public:
    TreeWidgetItem* parent() const;
    TreeWidgetItem* takeChild(int index);
    TreeWidgetItem();
    TreeWidgetItem* clone();

    void setStarred(bool starred) {
        this->starred = starred;
        if (starred) {
            // i->setText(1, "★");
            setIcon(1, QIcon(":/Resources/Icons/star.png"));
        } else {
            // i->setText(1, "");
            setIcon(1, QIcon());
        }
    }
    bool isStarred() {
        return this->starred;
    }

    QString message = "";
    int iconVectorIndex = 6;
    QDateTime lastEdited = QDateTime::currentDateTime();
    bool readOnly = false;
    QString toString();
    QStringList tags;
    bool highlighting = true;

private:
    bool starred = false;
};

#endif  // TREEWIDGETITEM_H
