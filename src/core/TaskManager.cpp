
#include "TaskManager.h"
#include <QDomDocument>
#include <QRandomGenerator>

/**
 * @brief Конструктор по-умолчанию Менеджера заданий.
 */
TaskManager::TaskManager()
    : m_loadStatus(LoadStatus::NOT_LOADED)
{}

/**
 * @brief Возвращает статус последней загрузки заданий (до вызова метода
 * TaskManager::load(const QString &) статус будет LoadStatus::NOT_LOADED).
 * @return Статус последней загрузки заданий.
 */
LoadStatus
TaskManager::getLoadStatus() const
{
    return m_loadStatus;
}

/**
 * @brief Производит попытку загрузить задание из заданного каталога.
 *
 * При существовании директории, путь к которой задан в аргументе #path,
 * происходит сканирование всех вложенных файлов формата "*.html".
 *
 * Из просканированных файлов в память загружаются все DOM-элементы, которые
 * описывают варианты заданий.
 *
 * В случае успешной загрзки или возникновения ошибки возвращается
 * соответствующий статус LoadStatus. Статус последней попытки загрузить
 * задания может быть получен при помощи метода
 * TaskManager::getLoadStatus() const.
 *
 * @param path Путь к директории (каталогу), где располагаются файлы заданий с
 * заготовленными вариантами задач.
 * @return Статус загрузки заданий.
 */
LoadStatus
TaskManager::load(const QString &path)
{
    m_taskList.clear();     /* отчистка старых заданий */
    /* ----- Чтение файлов с вариантами заданий ----- */
    QStringList filter("*.html");
    QDir dir(path);
    QStringList taskList = dir.entryList(filter, QDir::Files);
    for(const QString &task : taskList) {
        /* ----- Обработка каждого задания ----- */
        TaskDesc desc = this->readTaskFile( dir.absoluteFilePath(task) );
        desc.fileName = task;
        m_taskList.append(desc);    /* сохранение данных задания */
    }
    /* изменение статуса */
    if (taskList.isEmpty() == true) {
        m_loadStatus = LoadStatus::EMPTY;
    }
    else {
        m_loadStatus = LoadStatus::LOADED;
    }
    return this->getLoadStatus();
}

/**
 * @brief Производит генерацию заданий в соотвествии с настройками #settings.
 *
 * Создает каталог заданий, и подкаталоги для каждой группы. В каталогах групп
 * создаются файлы вариантов заданий. Номера вариантов задач для каждого
 * задания в пределах одной группы не повторяются.
 *
 * Генерация заданий происходит на основе предварительно загруженных заданий и
 * в соотвествии с заданными настройками #settings.
 *
 * В случае успешной генерации или непредвиденных ошибок в качестве результата
 * возвращается статус генерации GenStatus.
 *
 * @note Перед генерацией заданий необходимо успешно загрузить задание при
 * помощи метода TaskManager::load(const QString &).
 *
 * @param settings Настройки генерации заданий.
 * @return  Статус операции генерации заданий.
 */
GenStatus
TaskManager::generate(const GenSettings &settings)
{
    GenStatus status = GenStatus::SUCCESS;  /* статус генерации заданий */
    int maxVarCount = this->getMaxVarCount();
    /* ----- Проверка корректности настройки количества вариантов ----- */
    if ((0 < settings.varCount) && (settings.varCount <= maxVarCount)) {
        /* ----- Создание директории для заданий ----- */
        QDir dir(settings.path);
        if (dir.mkdir(settings.name) == true) {
            dir.cd(settings.name);
            /* ----- Создание заданий для каждой группы (независимо) ----- */
            int group = 0;      /* индекс группы */
            bool isOk = true;   /* индикатор ошибок генерации */
            auto grIt = settings.groups.begin();
            while ((grIt != settings.groups.end()) && isOk) {
                /* ----- Создание директории группы ----- */
                if (dir.mkdir( *grIt ) == true) {
                    dir.cd( *grIt );
                    /* ----- Генерация случайных номеров задач ----- */
                    QList<QList<int>> orders;
                    orders = genRandOrders(this->m_taskList.size(),
                                           settings.varCount);
                    /* ----- Создание файлов каждого варианта ----- */
                    for (int var = 0; var < settings.varCount; var++) {
                        QString name = settings.name +
                                       QString(" (вариант %1.%2)").arg(group+1)
                                                                  .arg(var+1);
                        QFile file( dir.filePath(name + ".html") );
                        if (file.open(QIODevice::WriteOnly) == true) {
                            /* ----- Формирование заданий ----- */
                            this->generateTaskVar(&file, name,
                                                  orders.takeFirst());
                            file.close();
                        }
                        else {
                            /* Не удается создать файл задания */
                            isOk = false;
                            status = GenStatus::CAN_NOT_CREATE_FILE;
                        }
                    }
                    dir.cdUp();     /* возврат в директорию всех групп */
                    group++;        /* увеличение индекса группы */
                }
                else {
                    /* Не удается создать директорию группы */
                    isOk = false;
                    status = GenStatus::CAN_NOT_CREATE_DIR;
                }
                grIt++;
            }
        }
        else {
            /* не удается создать каталог (директорию) */
            status = GenStatus::CAN_NOT_CREATE_DIR;
        }
    }
    else {
        /* некорректное число вариантов */
        status = GenStatus::WRONG_VARS_COUNT;
    }
    return status;
}

/**
 * @brief Производит расчет максимально дотупного числа вариантов, чтобы
 * задачи не повоторялись во всех заданиях.
 *
 * Вычисляет число вариантов, которые могут быть сформированы таким образом,
 * чтобы задания во всех задачах не повторялись. По определению, является
 * минимальным значением вариантов среди всех задач.
 *
 * @return Доступное число неповторяющихся вариантов.
 */
int
TaskManager::getMaxVarCount() const
{
    int min = 0;    /* по-умолчанию 0 вариантов */
    if (m_taskList.isEmpty() == false) {
        auto it = m_taskList.begin();
        min = it->vars.size();  /* min = первое значение */
        while (it != m_taskList.end()) {
            int size = it->vars.size();
            if (size < min) {
                min = size;
            }
            it++;
        }
    }
    return min;
}

/**
 * @brief Производит сканирование файла, путь к которому хранится в #path.
 * @param path Путь к сканируемому файлу.
 * @return Дескриптор задания, соответствующего просканированному файлу.
 */
TaskDesc
TaskManager::readTaskFile(const QString &path)
{
    TaskDesc description;   /* описание задания (результат подпрограммы) */
    QFile file(path);
    if (file.open(QIODevice::ReadOnly) == true) {
        QDomDocument doc;
        if (doc.setContent(&file) == true) {
            QDomNodeList nodeList = doc.elementsByTagName("div");
            for (int index = 0; index < nodeList.length(); index++) {
                QDomNode node = nodeList.item(index);
                if (node.isElement() && node.hasAttributes()) {
                    /* ----- Поиск информации задания по классу ----- */
                    QDomElement el = node.cloneNode(true).toElement();
                    QString classAttr = el.attribute("class");
                    if (classAttr == "task_body") {
                        /* найден вариант задания => добавляем в список */
                        description.vars.append(el);
                    }
                    else if (classAttr == "task_title") {
                        /* найдено название задания => запоминаем */
                        description.title = el;
                    }
                }
            }
        }
        else {
            description.status = TaskProcStatus::WRONG_FILE_STRUCT;
        }
        file.close();
        description.status = TaskProcStatus::SUCCES;
    }
    else {
        description.status = TaskProcStatus::CAN_NOT_OPEN_FILE;
    }

    return description;
}

/**
 * @brief Формирует списки случайных номеров задач каждого задания для
 * #varCount вараинтов.
 *
 * Формирует список, состоящий из #varCount элементов. Каждый элемент этого
 * списка также является списком, содержащим номера случайных задач для каждого
 * из #taskCount заданий.
 *
 * Случайные номера задач формируются таким образом, чтобы они не повторялись
 * в пределах одного задания среди всех сфоримрованных вариантов.
 *
 * @param taskCount Количетсво заданий.
 * @param varCount Количество вараинтов задач в каждом задании.
 * @return Списки случайных номеров задач каждого задания для #varCount
 * вариантов.
 */
QList<QList<int>>
TaskManager::genRandOrders(int taskCount, int varCount) const
{
    /* taskVarArr - матрица, которая содержит неповоряющиеся случайные номера
     * задач для каждого задания: в i-ой строке хранятся все номера задач
     * i-ого задания (в случайном порядке); а в j-ом столбце - случайные номера
     * задач для j-ого варианта */
    int *taskVarArr = new int[taskCount * varCount];

    /* ----- Генерация случайных вариантов для каждого задания ----- */
    for (int i = 0; i < taskCount; i++) {
        /* ----- Назначение каждому варианту номера (по возрастанию) ----- */
        for (int j = 0; j < varCount; j++) {
            *(taskVarArr + i * varCount + j) = j;
        }
        /* ----- Сортировка номеров в случайном пордяке ----- */
        for (int j = 0; j < varCount; j++) {;
            int k = QRandomGenerator::global()->bounded(varCount);
            qSwap( *(taskVarArr + i * varCount + j),
                   *(taskVarArr + i * varCount + k));
        }
    }

    /* ----- Формирование спсиков номеров задач для каждого варианта ----- */
    QList<QList<int>> varTaskList;
    for (int j = 0; j < varCount; j++) {
        QList<int> tasks;   /* список номеров задач для j-ого варианта */
        for (int i = 0; i < taskCount; i++) {
            tasks.append( *(taskVarArr + i * varCount + j) );
        }
        varTaskList.append( tasks );
    }

    delete [] taskVarArr;
    return varTaskList;
}

/**
 * @brief Генерирует 1 вараинт всех заданий в формате HTML и сохраняет его при
 * помощи #device.
 *
 * На основе загруженных заданий #m_taskList производится формирование 1
 * варианта заданий по шаблону resources/TaskTemplate.html.
 *
 * В формируемый вариант входит по 1 задаче для каждого задания. Номера
 * используемых задач задаются при помощи #randTaskVars.
 *
 * После формирования происходит запись (сохранение) при помощи #device.
 *
 * @warning Заголовок файла НЕ ДОЛЖЕН быть пустой строкой.
 * @warning Размер #randTaskVars должен быть НЕ МЕНЬШЕ размера #m_taskList.
 *
 * @param device Qt-устройство для записи (сохранения) сформированного задания.
 * @param titleStr Строка заголовка файла.
 * @param randTaskVars Список номеров задач каждого задания, которые учавствуют
 * в формировании варианта.
 */
void
TaskManager::generateTaskVar(QIODevice *device, const QString &titleStr,
                             QList<int> randTaskVars) const
{
    /* ----- Загрузка DOM-модели шаблона файла задания ----- */
    QFile tmpl(":/TaskTemplate.html");
    QDomDocument doc; doc.setContent(&tmpl);
    QDomElement title = doc.elementsByTagName("title").at(0).toElement();
    QDomNodeList divs = doc.elementsByTagName("div");
    QDomElement titleHolder = divs.at(1).toElement();
    QDomElement taskHolder = divs.at(2).toElement();

    /* ----- Добавление заголовка файла задания в DOM-модель ----- */
    QDomText titleTextNode = doc.createTextNode(titleStr);
    title.appendChild(titleTextNode.cloneNode(true));
    titleHolder.appendChild(titleTextNode);

    /* ----- Добавление заданий в DOM-модель ----- */
    for (const TaskDesc &desc : m_taskList) {
        /* формирование блока задания */
        QDomElement task = doc.createElement("div");
        task.setAttribute("class", "task");
        task.appendChild(desc.title.cloneNode(true));
        task.appendChild(desc.vars[randTaskVars.takeFirst()].cloneNode(true));
        /* вставка блока задания в DOM-модель */
        taskHolder.appendChild(task);
    }

    /* ----- Сохранение сформированного задания ----- */
    device->write( doc.toByteArray() );
}
