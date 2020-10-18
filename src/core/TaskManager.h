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
 * @brief Менеджер заданий, позволяет формировать различные варианты заданий.
 *
 * TaskManager служит для инкапсуляции процессов работы с заданиями. Основным
 * назначением является создание наборов заданий со случайными номерами задач.
 *
 * Сфоримровать наборы заданий можно при помощи метода:
 * GenStatus generate(const GenSettings &). Этот метод производит формирование
 * на основе загруженных вариантов задач, а также заданных настроек
 * GenSettings.
 *
 * Соответсвенно перед формированием заданий необходимо "загрузить" доступные
 * варианты задач при помощи метода LoadStatus load(const QString &).
 *
 * Узнать статус последней попытки загрузки доступных вариантов задач можно при
 * помощи метод LoadStatus getLoadStatus() const.
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
    void generateTaskVar(QIODevice *device, const QString &titleStr,
                         QList<int> randTaskVars) const;

    LoadStatus m_loadStatus;        ///< Статус менеджера заданий.
    QList<TaskDesc> m_taskList;     ///< Список загруженных заданий.
};
