
#include <gui/CreateTasksWid.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

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
    dirLayout->addWidget(dirBtn);
    layout->addLayout(dirLayout);
}
