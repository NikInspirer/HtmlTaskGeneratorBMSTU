#pragma once

#include <QWidget>
#include <core/TaskManager.h>
class QLineEdit;

class CreateTasksWid : public QWidget {
    Q_OBJECT

public:
    explicit CreateTasksWid(QWidget *parent = nullptr);

private Q_SLOTS:
    void openVarsDir();

private:
    QLineEdit *m_dirLE;     ///< Виджет для отображения каталога.
    TaskManager m_manager;  ///< Менеджер заданий.
};
