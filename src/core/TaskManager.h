#pragma once

#include <QDir>
#include <core/TaskInfo.h>

/**
 * @brief Статус менеджера заданий.
 */
enum class TaskManStatus {
    NOT_LOADED,                 ///< Варианты заданий не загруженны.
    ERROR_EMPTY,                ///< При загрузке не найдены задания.
    LOADED,                     ///< Варианты заданий загружены.
    ERROR_CAN_NOT_CREATE_FILE,  ///< Не удается создать файл итогового задания.
    CREATED                     ///< Задания сфоримрованы.
};

/**
 * @brief The TaskManager class
 */
class TaskManager {

public:
    explicit TaskManager();

    void load(const QString &taskPath);

    TaskManStatus getLoadStatus() const;

private:
    TaskDesc readTaskFile(const QString &path);

    TaskManStatus m_loadStatus;     ///< Статус менеджера заданий.
    QList<TaskDesc> m_taskList;     ///< Список загруженных заданий.
};
