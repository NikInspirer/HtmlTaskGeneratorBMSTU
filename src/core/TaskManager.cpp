
#include "TaskManager.h"
#include <QDomDocument>
#include <QDebug>

void
TaskManager::load(const QDir &dir)
{
    QStringList filter("*.html");
    QStringList taskList = dir.entryList(filter, QDir::Files);
    for(const QString &task : taskList) {
        this->readTaskFile( dir.absoluteFilePath(task) );
    }
}

void
TaskManager::readTaskFile(const QString &path)
{
    QFile file(path);
    if (file.open(QIODevice::ReadOnly) == true) {
        QDomDocument doc;
        if (doc.setContent(&file) == true) {
            QDomNodeList nodeList = doc.elementsByTagName("div");
            for (int index = 0; index < nodeList.length(); index++) {
                QDomNode node = nodeList.item(index);
                if (node.isElement() && node.hasAttributes()) {
                    /* ----- Поиск информации задания по классу ----- */
                    QDomElement el = node.toElement();
                    QString classAttr = el.attribute("class");
                    if (classAttr == "title") {
                        /* найдено название задания */
                        /* TODO: заполнение информации о названии */
                        qDebug() << "TASK:" << el.text();
                    }
                    if (classAttr == "task") {
                        /* найден вариант задания */
                        /* TODO: заполнение информации о варианте */
                        qDebug() << el.text();
                    }
                }
            }
        }
        file.close();
    }
}
