
#include "TaskManager.h"
#include <QDomDocument>
#include <QDebug>

TaskManager::TaskManager()
    : m_loadStatus(TaskLoadStatus::NOT_LOADED)
{}

void
TaskManager::load(const QString &taskPath)
{
    m_taskList.clear();     /* отчистка старых заданий */
    /* ----- Чтение файлов с вариантами заданий ----- */
    QStringList filter("*.html");
    QDir dir(taskPath);
    QStringList taskList = dir.entryList(filter, QDir::Files);
    for(const QString &task : taskList) {
        /* ----- Обработка каждого задания ----- */
        TaskDesc desc = this->readTaskFile( dir.absoluteFilePath(task) );
        desc.fileName = task;
        m_taskList.append(desc);    /* сохранение данных задания */
    }
    /* изменение статуса загрузки заданий */
    if (taskList.isEmpty() == true) {
        m_loadStatus = TaskLoadStatus::ERROR_EMPTY;
    }
    else {
        m_loadStatus = TaskLoadStatus::LOADED;
    }
}

TaskLoadStatus
TaskManager::getLoadStatus() const
{
    return m_loadStatus;
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
