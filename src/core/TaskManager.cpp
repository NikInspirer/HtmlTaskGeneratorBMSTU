
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
    QFile taskFile("out.html");
    taskFile.open(QIODevice::WriteOnly);
    this->generateTaskVar(&taskFile, this->genRandOrder(2));
    taskFile.close();
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
        if (count < task.vars.size()) {
            count = task.vars.size();
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
                        description.vars.append(el);
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

void
TaskManager::generateTaskVar(QIODevice *device, QList<int> randTaskVars)
{
    /* ----- Загрузка DOM-модели шаблона файла задания ----- */
    QFile tmpl(":/TaskTemplate.html");
    QDomDocument doc; doc.setContent(&tmpl);
    QDomElement title = doc.elementsByTagName("title").at(0).toElement();
    auto divs = doc.elementsByTagName("div");
    QDomElement titleHolder = divs.at(1).toElement();
    QDomElement taskHolder = divs.at(2).toElement();

    /* ----- Формирование заданий в DOM-моделе ----- */
    for (const TaskDesc &desc : m_taskList) {
        /* формирование названия задания */
        QDomElement taskTitle = doc.createElement("div");
        taskTitle.setAttribute("class", "task_title");
        taskTitle.appendChild( desc.title );
        /* формирование тела задания */
        QDomElement taskBody = doc.createElement("div");
        taskBody.setAttribute("class", "task_body");
        taskBody.appendChild(desc.vars[randTaskVars.takeFirst()]);
        /* формирование блока задания */
        QDomElement task = doc.createElement("div");
        task.setAttribute("class", "task");
        task.appendChild(taskTitle);
        task.appendChild(taskBody);
        /* вставка блока задания в DOM-модель */
        taskHolder.appendChild(task);
    }

    /* ----- Сохранение сформированного задания ----- */
    QTextStream stream(device);
    stream << doc.toString();
}
