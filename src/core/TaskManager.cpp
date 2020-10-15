
#include "TaskManager.h"
#include <QDomDocument>
#include <QDebug>

void
TaskManager::load(const QDir &dir)
{
    QStringList filter("*.html");
    QStringList taskList = dir.entryList(filter, QDir::Files);
    for(const QString &task : taskList) {
        TaskDesc desc = this->readTaskFile( dir.absoluteFilePath(task) );
        desc.fileName = task;

        // TODO: сохранение данных задания в памяти + СТАТУС
        qDebug() << desc.taskTitle.toElement().text();
        for (QDomNode t : desc.taskList) {
            qDebug() << t.toElement().text();
        }
    }
}

TaskDesc
TaskManager::readTaskFile(const QString &path)
{
    TaskDesc description;   /* описание задания (результат подпрограммы) */
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
                        /* найдено название задания => запоминаем */
                        description.taskTitle = el;
                    }
                    if (classAttr == "task") {
                        /* найден вариант задания => добавляем в список */
                        description.taskList.append(el);
                    }
                }
            }
        }
        else {
            description.status = TaskProcStatus::WRONG_FILE_STRUCT;
        }
        file.close();
        description.status = TaskProcStatus::SUCCES;
    }
    else {
        description.status = TaskProcStatus::CAN_NOT_OPEN_FILE;
    }

    return description;
}
