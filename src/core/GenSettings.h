#pragma once

#include <QString>
#include <QStringList>

/**
 * @brief Настройки процесса генерации заданий.
 */
struct GenSettings {
    QString path;       ///< Директория, где происходит генерация.
    QString name;       ///< Название групп заданий (каталог).
    int varCount;       ///< Количество генерируемых вариантов.
    QStringList groups; ///< Названия групп, для которых формируются задания.
};

/**
 * @brief Статус генерации вариантов заданий.
 */
enum class GenStatus {
    SUCCESS,            ///< Успешное создание заданий.
    WRONG_VARS_COUNT,   ///< Невозможно сфоримровать заданное кол-во вариантов.
    CAN_NOT_CREATE_DIR, ///< Не удается создать директорию.
    CAN_NOT_CREATE_FILE,    ///< Не удается создать файлы заданий.
};