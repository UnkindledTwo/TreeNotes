#include "saver.h"

Saver::Saver(TreeWidget *noteTree, QVector<QIcon> iconVector)
{
    this->noteTree = noteTree;
    this->iconVector = iconVector;
}

void Saver::ReadChildren(QDomDocument *doc, QDomNode node, TreeWidgetItem *parent){
    if(!node.isElement()) return;
    if(!node.toElement().hasAttribute("message")) return;
    if(node.toElement().tagName() == "") return;

    parent->message = node.toElement().attribute("message");
    parent->setText(0,node.toElement().attribute("title"));

    QDomNodeList itemList = node.toElement().elementsByTagName("NoteItem");
    for(int i = 0; i < node.toElement().childNodes().count(); i++){
    //for(int i = 0; i < itemList.count(); i++){
        QDomElement currentElement = node.toElement().childNodes().at(i).toElement();
        if(currentElement.tagName() != "NoteItem") continue;
        TreeWidgetItem *newItem = new TreeWidgetItem();

        newItem->setIcon(0 , iconVector.at(currentElement.attribute("icon").toInt()));
        newItem->iconVectorIndex = currentElement.attribute("icon").toInt();
        newItem->lastEdited = QDateTime::fromString(currentElement.attribute("lastEdited"));
        newItem->starred = qvariant_cast<bool>(currentElement.attribute("starred", "0"));
        newItem->readOnly = qvariant_cast<bool>(currentElement.attribute("readOnly", "0"));

        QDomNode domTags = currentElement.childNodes().at(0);
        for(int i = 0; i < domTags.childNodes().count(); i++) {
            QDomElement domTag = domTags.childNodes().at(i).toElement();
            newItem->tags.append(domTag.attribute("name"));
        }

        parent->addChild(newItem);
        noteTree->setStar(newItem, newItem->starred);
        ReadChildren(doc, node.toElement().childNodes().at(i), newItem);
    }
}

void Saver::AddChildren(QDomDocument *doc, QDomElement *elem, QTreeWidgetItem *parent){
    QTreeWidgetItemIterator it(noteTree);
    while(*it){
        if((*it)->parent() == parent){
            QDomElement newElem = doc->createElement("NoteItem");
            newElem.setAttribute("message", ((TreeWidgetItem*)(*it))->message);
            newElem.setAttribute("title", ((TreeWidgetItem*)(*it))->text(0));
            newElem.setAttribute("icon", ((TreeWidgetItem*)(*it))->iconVectorIndex);
            newElem.setAttribute("lastEdited", ((TreeWidgetItem*)(*it))->lastEdited.toString());
            newElem.setAttribute("starred", (((TreeWidgetItem*)(*it))->starred));
            newElem.setAttribute("readOnly", ((TreeWidgetItem*)(*it))->readOnly);

            QDomElement domTags = doc->createElement("Tags");
            foreach (QString tag, ((TreeWidgetItem*)(*it))->tags) {
                QDomElement domTag = doc->createElement("Tag");
                domTag.setAttribute("name", tag);
                domTags.appendChild(domTag);
            }
            newElem.appendChild(domTags);


            elem->appendChild(newElem);
            AddChildren(doc, &newElem, *it);
        }
        it++;
    }
}

void Saver::SaveToFile(){
    qDebug() << "Saving to file";
    QDomDocument document;

    QDomElement root = document.createElement("All_Notes");

    document.appendChild(root);

    QTreeWidgetItemIterator it(noteTree);
    while(*it){
        if((*it)->parent() == 0x0){

            QDomElement elem = document.createElement("NoteItem");
            elem.setAttribute("message", ((TreeWidgetItem*)(*it))->message);
            elem.setAttribute("title", (*it)->text(0));
            elem.setAttribute("icon", ((TreeWidgetItem*)(*it))->iconVectorIndex);
            elem.setAttribute("lastEdited", ((TreeWidgetItem*)(*it))->lastEdited.toString());
            elem.setAttribute("starred", (((TreeWidgetItem*)(*it))->starred));
            elem.setAttribute("readOnly", ((TreeWidgetItem*)(*it))->readOnly);

            QDomElement domTags = document.createElement("Tags");
            foreach (QString tag, ((TreeWidgetItem*)(*it))->tags) {
                QDomElement domTag = document.createElement("Tag");
                domTag.setAttribute("name", tag);
                domTags.appendChild(domTag);
            }
            elem.appendChild(domTags);

            AddChildren(&document, &elem, (*it));
            root.appendChild(elem);
        }
        it++;
    }

    QString savePath = qApp->applicationDirPath()+ "/" + saveFileName;
    QFileInfo saveFileInfo(savePath);
    QDir savePathDir(saveFileInfo.absoluteDir());
    if(!savePathDir.exists()) {
        savePathDir.mkdir(savePathDir.absolutePath());
    }
    QFile file(savePath);
    file.open(QIODevice::WriteOnly);
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    stream << document.toString(4);

    QFile fileB64(qApp->applicationDirPath() + "/" + saveFileName + ".b64");
    fileB64.open(QIODevice::WriteOnly);
    QTextStream streamB64(&fileB64);
    streamB64.setCodec("UTF-8");
    streamB64 << document.toString(4).toUtf8().toBase64();
    fileB64.close();
    streamB64.flush();

    file.close();
    stream.flush();

    noteTree->setEnabled(false);

    noteTree->setEnabled(true);
    qDebug() << "Saving to file finished" << qApp->applicationDirPath()+ "/" + saveFileName;
}

void Saver::ReadFromFile(){
    qDebug() << "Reading from file";
    noteTree->clear();

    QString errMsg;
    int errLine;
    int errColumn;

    QDomDocument document;
    QFile file(qApp->applicationDirPath() + "/" + saveFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // Error while loading file
        qDebug() << "Error while loading file";
        return;
    }

    if(!document.setContent(file.readAll(), false, &errMsg, &errLine, &errColumn)){
        qDebug() << "Error while setting xmldoc content: " << errMsg << QString::number(errLine) << QString::number(errColumn);
    }
    file.close();

    QDomNodeList childElementList = document.childNodes();
    for(int j = 0; j < childElementList.count(); j++){
        QDomElement root = childElementList.at(j).toElement();
        QDomNodeList itemList = root.elementsByTagName("NoteItem");
        for(int i = 0; i < root.toElement().childNodes().count(); i++){
            if(!itemList.at(i).toElement().hasAttribute("message")) continue;
            if(itemList.at(i).toElement().attribute("title") == "") continue;

            QDomElement currentElement = root.toElement().childNodes().at(i).toElement();
            if(currentElement.tagName() != "NoteItem") continue;
            TreeWidgetItem *newItem = new TreeWidgetItem();
            newItem->setIcon(0 , iconVector.at(currentElement.attribute("icon").toInt()));
            newItem->iconVectorIndex = (currentElement.attribute("icon").toInt());
            newItem->lastEdited = QDateTime::fromString(currentElement.attribute("lastEdited"));
            newItem->starred = qvariant_cast<bool>(currentElement.attribute("starred", "0"));
            newItem->readOnly = qvariant_cast<bool>(currentElement.attribute("readOnly", "0"));

            QDomNode domTags = currentElement.childNodes().at(0);
            for(int i = 0; i < domTags.childNodes().count(); i++) {
                QDomElement domTag = domTags.childNodes().at(i).toElement();
                newItem->tags.append(domTag.attribute("name"));
            }

            noteTree->addTopLevelItem(newItem);
            noteTree->setStar(newItem, newItem->starred);
            ReadChildren(&document, root.toElement().childNodes().at(i), newItem);
        }
    }

    qDebug() << "Reading from file finished" << file.fileName();
}
