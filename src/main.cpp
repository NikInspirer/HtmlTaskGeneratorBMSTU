
#include <QApplication>
#include <gui/CreateTasksWid.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    CreateTasksWid wid;
    wid.show();

    return app.exec();
}
