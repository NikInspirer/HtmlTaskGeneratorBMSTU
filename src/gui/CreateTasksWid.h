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

private:
    void resetLoadStatus();

    QLineEdit *m_inDirLE;   ///< Виджет для отображения каталога вараинтов.
    QLabel *m_loadStatusL;  ///< Label статуса загрузки вараинтов.
    QLineEdit *m_nameLE;    ///< Поле ввода названия заданий.
    QSpinBox *m_varCountSB; ///< Поле ввода количества вариантов заданий.
    QTextEdit *m_groupsTE;  ///< Поле ввода названий групп.
    QLineEdit *m_outDirLE;  ///< Виджет для отображения выходного каталога.
    QPushButton *m_outDirPB;///< Кнопка выбора выходного каталога.
    QPushButton *m_createPB;///< Кнопка создания заданий.
    QWidget *m_settingsWid; ///< Виджет настроек (для быстрого вкл./выкл.).
    TaskManager m_manager;  ///< Менеджер заданий.
};
