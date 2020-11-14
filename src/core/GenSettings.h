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
    CAN_NOT_READ_IMG,       ///< Не удается найти файл изображения.
};

/**
 * @brief Функция преобразвования GenStatus в QString.
 * @param status Статус генерации вариантов заданий для преобразования.
 * @return Строка статуса генерации вариантов заданий.
 */
inline QString
toString(GenStatus status)
{
    QString str;
    switch (status)
    {
    case GenStatus::SUCCESS:
       str = "Успешное создание заданий";
       break;
    case GenStatus::WRONG_VARS_COUNT:
       str = "Невозможно сфоримровать заданное количество вариантов";
       break;
    case GenStatus::CAN_NOT_CREATE_DIR:
       str = "Не удается создать директорию";
       break;
    case GenStatus::CAN_NOT_CREATE_FILE:
       str = " Не удается создать файлы заданий";
       break;
    case GenStatus::CAN_NOT_READ_IMG:
       str = "Не удается найти файл изображения";
       break;
    }
    return str;
}
