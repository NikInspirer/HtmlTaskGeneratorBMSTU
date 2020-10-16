#pragma once

#include <QString>
#include <QDomNode>
#include <QList>

/**
 * @brief Статус обработки файла задания.
 */
enum class TaskProcStatus {
    SUCCES,                 ///< Успешно прочитано.
    CAN_NOT_OPEN_FILE,      ///< Не удалось открыть файл.
    WRONG_FILE_STRUCT       ///< Неправильная структура файла.
};

/**
 * @brief Дескриптор задания.
 */
struct TaskDesc {
    QString fileName;           ///< Название файла задания.
    TaskProcStatus status;      ///< Статус обраотки файла задания.
    QDomNode title;             ///< DOM-элемент заголовка задания.
    QList<QDomNode> varList;    ///< Список DOM-элементов вариантов задания.
};
