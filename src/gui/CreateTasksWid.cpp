
#include <gui/CreateTasksWid.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QDir>
#include <QDebug>

CreateTasksWid::CreateTasksWid(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    /* ----- Текст каталога с вариантами задач ----- */
    layout->addWidget( new QLabel(tr("Каталог с вариантами задач:")) );

    /* ----- Область отображения каталога ----- */
    QHBoxLayout *dirLayout = new QHBoxLayout;
    m_dirLE = new QLineEdit(tr("Каталог не выбран..."));
    m_dirLE->setReadOnly(true);
    dirLayout->addWidget(m_dirLE);
    QPushButton *dirBtn = new QPushButton(tr("Открыть"));
    dirBtn->setToolTip( tr("Выбрать каталог с вариантами задач") );
    connect(dirBtn, &QPushButton::clicked,
            this, &CreateTasksWid::openVarsDir);
    dirLayout->addWidget(dirBtn);
    layout->addLayout(dirLayout);
}

/**
 * @brief [private slot] Обработки нажатия на кнопку открытия новой директории
 * с вариантами задач.
 */
void
CreateTasksWid::openVarsDir()
{
    QString path = QFileDialog::getExistingDirectory(this,
                                  tr("Выберите каталог с вариантами заданий"));
    if (path.isEmpty() == false)
    {
        /* ----- Файл выбран ----- */
        m_dirLE->setText( path );
        m_manager.load(path);
    }
}
