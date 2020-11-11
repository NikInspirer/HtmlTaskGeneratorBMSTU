
#include <gui/CreateTasksWid.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>
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
    m_inDirLE = new QLineEdit(tr("Каталог не выбран..."));
    m_inDirLE->setReadOnly(true);
    dirLayout->addWidget(m_inDirLE);
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

    /* ----- Область настроек ----- */
    QGridLayout *setLayout = new QGridLayout;
    setLayout->addWidget(new QLabel("Название задания:"), 1, 0);
    m_nameLE = new QLineEdit;
    setLayout->addWidget(m_nameLE, 1, 1, Qt::AlignLeft);
    setLayout->addWidget(new QLabel("Количество вариантов:"), 2, 0);
    m_varCountSB = new QSpinBox;
    setLayout->addWidget(m_varCountSB, 2, 1);
    setLayout->addWidget(new QLabel("Названия групп (разделенные '\\n'):"),
                         3, 0, 1, 2);
    m_groupsTE = new QTextEdit;
    setLayout->addWidget(m_groupsTE, 4, 0, 1, 2);
    setLayout->addWidget(new QLabel("Выходной каталог:"), 5, 0, 1, 2);
    QHBoxLayout *outDirLayout = new QHBoxLayout;
    m_outDirLE = new QLineEdit;
    m_outDirLE->setReadOnly(true);
    outDirLayout->addWidget(m_outDirLE);
    m_outDirPB = new QPushButton(tr("Выбрать"));
    outDirLayout->addWidget(m_outDirPB);
    setLayout->addLayout(outDirLayout, 6, 0, 1, 2);
    m_createPB = new QPushButton(tr("Сформировать задания"));
    setLayout->addWidget(m_createPB, 7, 0, 1, 2);
    layout->addLayout(setLayout);
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
        m_inDirLE->setText( path );
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
