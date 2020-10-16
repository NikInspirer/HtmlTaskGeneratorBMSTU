
#include "TaskManager.h"
#include <QDomDocument>
#include <QRandomGenerator>
#include <QDebug>

TaskManager::TaskManager()
    : m_loadStatus(TaskManStatus::NOT_LOADED)
{}

void
TaskManager::load(const QString &path)
{
    m_taskList.clear();     /* отчистка старых заданий */
    /* ----- Чтение файлов с вариантами заданий ----- */
    QStringList filter("*.html");
    QDir dir(path);
    QStringList taskList = dir.entryList(filter, QDir::Files);
    for(const QString &task : taskList) {
        /* ----- Обработка каждого задания ----- */
        TaskDesc desc = this->readTaskFile( dir.absoluteFilePath(task) );
        desc.fileName = task;
        m_taskList.append(desc);    /* сохранение данных задания */
    }
    /* изменение статуса */
    if (taskList.isEmpty() == true) {
        m_loadStatus = TaskManStatus::ERROR_EMPTY;
    }
    else {
        m_loadStatus = TaskManStatus::LOADED;
    }
}

void
TaskManager::generate(const GenSettings &settings)
{
    /* ----- Формирование для каждой группы ----- */
    for (int iteration = 0; iteration < settings.groupCount; iteration++) {
        /* ----- Проход по каждому заданию ----- */
        for (const TaskDesc &task : m_taskList) {
            /* ----- Формирование варианта ----- */
            QList<int> randOrder = this->genRandOrder(settings.varCount);
            for (int var : randOrder) {
                const QDomNode &varEl = task.varList.at(var);
                qDebug() << varEl.toElement().text();
            }
        }
    }
}

TaskManStatus
TaskManager::getLoadStatus() const
{
    return m_loadStatus;
}

int
TaskManager::getMaxVarCount() const
{
    int count = 0;
    for (const TaskDesc &task : m_taskList) {
        if (count < task.varList.size()) {
            count = task.varList.size();
        }
    }
    return count;
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
                        description.title = el;
                    }
                    if (classAttr == "task") {
                        /* найден вариант задания => добавляем в список */
                        description.varList.append(el);
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

QList<int>
TaskManager::genRandOrder(int len) const
{
    int *order = new int[len];
    /* наполнение индексов в порядке возрастания */
    for (int n = 0; n < len; n++) order[n] = n;
    /* сортировка в случайном порядке */
    for (int i = 0; i < len; i++) {
        int j = QRandomGenerator::global()->bounded(len);
        qSwap(order[i], order[j]);
    }
    /* формирование списка */
    QList<int> orderList;
    for (int n = 0; n < len; n++) orderList.append(order[n]);
    delete [] order;
    return orderList;
}
