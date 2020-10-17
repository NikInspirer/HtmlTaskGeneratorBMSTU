
#include "TaskManager.h"
#include <QDomDocument>
#include <QRandomGenerator>
#include <QDebug>

TaskManager::TaskManager()
    : m_loadStatus(LoadStatus::NOT_LOADED)
{}

LoadStatus
TaskManager::getLoadStatus() const
{
    return m_loadStatus;
}

LoadStatus
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
        m_loadStatus = LoadStatus::EMPTY;
    }
    else {
        m_loadStatus = LoadStatus::LOADED;
    }
    return this->getLoadStatus();
}

GenStatus
TaskManager::generate(const GenSettings &settings)
{
    QFile taskFile("out.html");
    taskFile.open(QIODevice::WriteOnly | QIODevice::Text);
    this->generateTaskVar(&taskFile, this->genRandOrder(2));
    taskFile.close();
    return GenStatus::SUCCESS;
}

/**
 * @brief Производит расчет максимально дотупного числа вариантов, чтобы
 * задачи не повоторялись во всех заданиях.
 *
 * Вычисляет число вариантов, которые могут быть сформированы таким образом,
 * чтобы задания во всех задачах не повторялись. По определению, является
 * минимальным значением вариантов среди всех задач.
 *
 * @return Доступное число неповторяющихся вариантов.
 */
int
TaskManager::getMaxVarCount() const
{
    int min = 0;    /* по-умолчанию 0 вариантов */
    if (m_taskList.isEmpty() == false) {
        auto it = m_taskList.begin();
        min = it->vars.size();  /* min = первое значение */
        while (it != m_taskList.end()) {
            int size = it->vars.size();
            if (size < min) {
                min = size;
            }
        }
    }
    return min;
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
                    if (classAttr == "task_body") {
                        /* найден вариант задания => добавляем в список */
                        description.vars.append(el);
                    }
                    else if (classAttr == "task_title") {
                        /* найдено название задания => запоминаем */
                        description.title = el;
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

    /* ----- Добавление заданий в DOM-модель ----- */
    for (const TaskDesc &desc : m_taskList) {
        /* формирование блока задания */
        QDomElement task = doc.createElement("div");
        task.setAttribute("class", "task");
        task.appendChild(desc.title);
        task.appendChild(desc.vars[randTaskVars.takeFirst()]);
        /* вставка блока задания в DOM-модель */
        taskHolder.appendChild(task);
    }

    /* ----- Сохранение сформированного задания ----- */
    device->write( doc.toByteArray() );
}
