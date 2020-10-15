#pragma once

#include <QDir>

/**
 * @brief The TaskManager class
 */
class TaskManager {

public:
    explicit TaskManager() {};
    void load(const QDir &dir);

private:
    void readTaskFile(const QString &path);
};
