#pragma once

#include <QWidget>
class QLineEdit;

class CreateTasksWid : public QWidget {
    Q_OBJECT

public:
    explicit CreateTasksWid(QWidget *parent = nullptr);

private:
    QLineEdit *m_dirLE;     ///< Виджет для отображения каталога.
};
