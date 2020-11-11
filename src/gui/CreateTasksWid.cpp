
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
    connect(dirBtn, &QPushButton::clicked, this, &CreateTasksWid::loadVars);
    dirLayout->addWidget(dirBtn);
    layout->addLayout(dirLayout);
    /* ----- Статус загрузки вариантов ----- */
    QHBoxLayout *statusLayout = new QHBoxLayout;
    QLabel *statusL = new QLabel(tr("Статус загрузки: "));
    m_loadStatusL = new QLabel();
    statusLayout->addWidget(statusL);
    statusLayout->addWidget(m_loadStatusL);
    statusLayout->addStretch(1);
    layout->addLayout(statusLayout);
    this->resetLoadStatus();
}

/**
 * @brief [private slot] Обработки нажатия на кнопку открытия новой директории
 * с вариантами задач.
 */
void
CreateTasksWid::loadVars()
{
    QString path = QFileDialog::getExistingDirectory(this,
                                  tr("Выберите каталог с вариантами заданий"));
    if (path.isEmpty() == false)
    {
        /* ----- Файл выбран ----- */
        m_dirLE->setText( path );
        m_manager.load(path);
        this->resetLoadStatus();
    }
}

void
CreateTasksWid::resetLoadStatus()
{
    QString st;
    switch(m_manager.getLoadStatus())
    {
    case LoadStatus::NOT_LOADED:        /* не загружено */
        st = tr("НЕ ЗАГРУЖЕНЫ ВАРИАНТЫ");
        break;
    case LoadStatus::EMPTY:             /* нет вариантов */
        st = tr("НЕТ ВАРИАНТОВ ЗАДАНИЙ В КАТАЛОГЕ");
        break;
    case LoadStatus::LOADED:            /* загружено */
        st = tr("ВАРИАНТЫ ЗАДАНИЙ ЗАГРУЖЕНЫ");
        break;
    }
    m_loadStatusL->setText(st);
}

void
CreateTasksWid::setEnableForCreatingVars(bool isEnable)
{
}
