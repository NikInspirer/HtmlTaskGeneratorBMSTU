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
 * @brief Настройки процесса генерации заданий.
 */
struct GenSettings {
    QString path;       ///< Директория, где происходит генерация.
    QString name;       ///< Название групп заданий (каталог).
    int varCount;       ///< Количество генерируемых вариантов.
    int groupCount;     ///< Количество групп заданий.
};

/**
 * @brief The TaskManager class
 */
class TaskManager {

public:
    explicit TaskManager();

    void load(const QString &path);
    void generate(const GenSettings &settings);
    TaskManStatus getLoadStatus() const;
    int getMaxVarCount() const;

private:
    TaskDesc readTaskFile(const QString &path);
    QList<int> genRandOrder(int len) const;

    void generateTaskVar(QIODevice *device, QList<int> randTaskVars);

    TaskManStatus m_loadStatus;     ///< Статус менеджера заданий.
    QList<TaskDesc> m_taskList;     ///< Список загруженных заданий.
};
