
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
#include <QMessageBox>
#include <QDebug>

CreateTasksWid::CreateTasksWid(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    /* ----- Шрифт заголовков ----- */
    QFont titleFont = this->font();
    titleFont.setPointSize(titleFont.pointSize() + 2);
    titleFont.setUnderline(true);
    titleFont.setBold(true);

    /* ----- Выбор каталога с вариантами задач ----- */
    QLabel *inDirTitleL = new QLabel(tr("Каталог вариантов задач"));
    inDirTitleL->setFont(titleFont);
    layout->addWidget(inDirTitleL, 0, Qt::AlignCenter);
    layout->addWidget( new QLabel(tr("Каталог с вариантами задач:")) );
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
    m_settingsWid = new QWidget;
    QGridLayout *setLayout = new QGridLayout(m_settingsWid);
    setLayout->setContentsMargins(0, 0, 0, 0);
    QLabel *setTitleL = new QLabel(tr("Настройки генерации заданий"));
    setTitleL->setFont(titleFont);
    setLayout->addWidget(setTitleL, 0, 0, 1, 2, Qt::AlignCenter);
    setLayout->addWidget(new QLabel("Название задания:"), 1, 0);
    m_nameLE = new QLineEdit;
    setLayout->addWidget(m_nameLE, 1, 1, Qt::AlignLeft);
    setLayout->addWidget(new QLabel("Количество вариантов:"), 2, 0);
    m_varCountSB = new QSpinBox;
    m_varCountSB->setMinimum(1);
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
    QPushButton *outDirPB = new QPushButton(tr("Выбрать"));
    connect(outDirPB, &QPushButton::clicked,
            this, &CreateTasksWid::selectOutDir);
    outDirLayout->addWidget(outDirPB);
    setLayout->addLayout(outDirLayout, 6, 0, 1, 2);
    QPushButton *createPB = new QPushButton(tr("Сформировать задания"));
    connect(createPB, &QPushButton::clicked,
            this, &CreateTasksWid::createTasks);
    setLayout->addWidget(createPB, 7, 0, 1, 2);
    layout->addWidget(m_settingsWid);
    m_settingsWid->setEnabled(false);
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
        if (m_manager.getLoadStatus() == LoadStatus::LOADED)
        {
            /* Активация настроек */
            m_settingsWid->setEnabled(true);
            /* Настройка доступного количества вариантов */
            m_varCountSB->setMaximum( m_manager.getMaxVarCount() );
            m_varCountSB->setValue( m_varCountSB->maximum() );
        }
        else
        {
            m_settingsWid->setEnabled(false);
        }
    }
}

/**
 * @brief [private slot] Обработка нажатия кнопки выбора выходного каталога.
 *
 * Запускает выбор каталога. Если каталог выбран, то устанавливает его путь в
 * качестве значения для #m_outDirLE.
 */
void
CreateTasksWid::selectOutDir()
{
    QString path = QFileDialog::getExistingDirectory(this,
                                  tr("Выберите каталог для сохранения заданий"),
                                                     m_inDirLE->text());
    if (path.isEmpty() == false)
    {
        m_outDirLE->setText( path );
    }
}

/**
 * @brief [private slot] Производит попытку формирования файлов заданий для
 * каждой группы.
 */
void
CreateTasksWid::createTasks()
{
    QStringList errors;
    if (this->checkSettings(&errors) == true)
    {
        /* ----- Ошибок нет => формирование задания ----- */
        GenSettings settings;
        settings.name = m_nameLE->text();
        settings.varCount = m_varCountSB->value();
        settings.groups = this->parseGroupNames();
        settings.path = m_outDirLE->text();
        GenStatus status = m_manager.generate(settings);
        if (status == GenStatus::SUCCESS)
        {
            QMessageBox::information(this, tr("Задания созданы"),
                                     tr("Задания успешно созданы в "
                                        "соответсвии с настройками."));
        }
        else
        {
            QMessageBox::critical(this, tr("Ошибка создания"),
                                  tr("Задания сформированны не полностью.\n\n"
                                     "ОШИБКА:\n") + toString(status));
        }
    }
    else
    {
        /* ----- Найдены ошибки => вывод сообщения ----- */
        QMessageBox::critical(this, tr("Проблемы с настройками"),
                              errors.join("\n"));
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

/**
 * @brief Производит парсинг названий групп из #m_groupsTE по символам
 * '\n', '\r'.
 * @return Список названий групп.
 */
QStringList
CreateTasksWid::parseGroupNames() const
{
    return m_groupsTE->toPlainText().split(QRegExp("[\r\n]"),
                                           Qt::SkipEmptyParts);
}

/**
 * @brief Производит проверку введеных данных настроек генерации заданий.
 * @param errors[out] Если errors != nulltr, то записывает в него сообщения
 * об ошибках.
 * @return true - если все поля корректные; инчае - false.
 */
bool
CreateTasksWid::checkSettings(QStringList *errors) const
{
    bool isCorrect = true;
    /* ----- Проверка названия задания ----- */
    if (m_nameLE->text().isEmpty() == true)
    {
        isCorrect = false;
        if (errors) { errors->append(tr("Не указано название задания.")); }
    }
    /* ----- Проверка названий групп ----- */
    if (this->parseGroupNames().isEmpty() == true)
    {
        isCorrect = false;
        if (errors) { errors->append(tr("Не удалось разобрать названия групп.")); }
    }
    /* ----- Проверка выходного каталога ----- */
    if (m_outDirLE->text().isEmpty() == true)
    {
        isCorrect = false;
        if (errors) { errors->append(tr("Не выбран выходной каталог.")); }
    }
    return isCorrect;
}
