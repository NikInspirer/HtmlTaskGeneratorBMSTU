#pragma once

#include <QDir>
#include <core/TaskInfo.h>

/**
 * @brief Статус загрузки вариантов заданий.
 */
enum class TaskLoadStatus {
    NOT_LOADED,     ///< Варианты заданий не загруженны.
    LOADED,         ///< Варианты заданий загружены.
    CREATED         ///< Задания сфоримрованы.
};

/**
 * @brief The TaskManager class
 */
class TaskManager {

public:
    explicit TaskManager();
    void load(const QDir &dir);

private:
    TaskDesc readTaskFile(const QString &path);

    TaskLoadStatus m_loadStatus;    ///< Статус загрузки вариантов заданий.
    QList<TaskDesc> m_taskList;     ///< Список загруженных заданий.
};
