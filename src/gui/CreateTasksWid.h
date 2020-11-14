#pragma once

#include <QWidget>
#include <core/TaskManager.h>
class QLineEdit;
class QLabel;
class QSpinBox;
class QTextEdit;
class QPushButton;

class CreateTasksWid : public QWidget {
    Q_OBJECT

public:
    explicit CreateTasksWid(QWidget *parent = nullptr);

private Q_SLOTS:
    void loadVars();
    void selectOutDir();
    void createTasks();

private:
    void resetLoadStatus();
    QStringList parseGroupNames() const;
    bool checkSettings(QStringList *errors = nullptr) const;

    QLineEdit *m_inDirLE;   ///< Виджет для отображения каталога вараинтов.
    QLabel *m_loadStatusL;  ///< Label статуса загрузки вараинтов.
    QLineEdit *m_nameLE;    ///< Поле ввода названия заданий.
    QSpinBox *m_varCountSB; ///< Поле ввода количества вариантов заданий.
    QTextEdit *m_groupsTE;  ///< Поле ввода названий групп.
    QLineEdit *m_outDirLE;  ///< Виджет для отображения выходного каталога.
    QWidget *m_settingsWid; ///< Виджет настроек (для быстрого вкл./выкл.).
    TaskManager m_manager;  ///< Менеджер заданий.
};
