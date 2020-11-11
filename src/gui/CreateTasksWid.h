#pragma once

#include <QWidget>
#include <core/TaskManager.h>
class QLineEdit;
class QLabel;

class CreateTasksWid : public QWidget {
    Q_OBJECT

public:
    explicit CreateTasksWid(QWidget *parent = nullptr);

private Q_SLOTS:
    void loadVars();

private:
    void resetLoadStatus();
    void setEnableForCreatingVars(bool isEnable);

    QLineEdit *m_dirLE;     ///< Виджет для отображения каталога.
    QLabel *m_loadStatusL;  ///< Label статуса загрузки вараинтов.
    TaskManager m_manager;  ///< Менеджер заданий.
};
