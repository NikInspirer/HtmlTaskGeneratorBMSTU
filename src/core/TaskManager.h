#pragma once

#include <QDir>
#include <core/TaskInfo.h>

/**
 * @brief The TaskManager class
 */
class TaskManager {

public:
    explicit TaskManager() {};
    void load(const QDir &dir);

private:
    TaskDesc readTaskFile(const QString &path);
};
