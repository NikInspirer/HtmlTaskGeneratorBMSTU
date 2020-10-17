#pragma once

#include <QDir>
#include <core/TaskInfo.h>
#include <core/GenSettings.h>

/**
 * @brief Статус загрузки заданий.
 */
enum class LoadStatus {
    NOT_LOADED,     ///< Варианты заданий не загруженны.
    EMPTY,          ///< При загрузке не найдены задания.
    LOADED,         ///< Варианты заданий загружены.
};

/**
 * @brief The TaskManager class
 */
class TaskManager {

public:
    explicit TaskManager();

    LoadStatus getLoadStatus() const;
    LoadStatus load(const QString &path);
    GenStatus generate(const GenSettings &settings);
    int getMaxVarCount() const;

private:
    TaskDesc readTaskFile(const QString &path);
    QList<QList<int>> genRandOrders(int taskCount, int varCount) const;
    void generateTaskVar(QIODevice *device, QList<int> randTaskVars);

    LoadStatus m_loadStatus;     ///< Статус менеджера заданий.
    QList<TaskDesc> m_taskList;     ///< Список загруженных заданий.
};
