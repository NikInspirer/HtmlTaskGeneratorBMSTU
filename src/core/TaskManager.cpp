
#include "TaskManager.h"
#include <QDomDocument>
#include <QDebug>

void
TaskManager::load(const QDir &dir)
{
    QStringList filter("*.html");
    QStringList taskList = dir.entryList(filter, QDir::Files);
    for(const QString &task : taskList) {
        QFile file( dir.absoluteFilePath(task) );
        if (file.open(QIODevice::ReadOnly) == true) {
            QDomDocument doc;
            if (doc.setContent(&file) == true) {
                QDomElement root = doc.documentElement();
                QDomElement title = root.elementsByTagName("title")
                                        .at(0).toElement();
                qDebug() << title.text();
            }
        }
    }
}
