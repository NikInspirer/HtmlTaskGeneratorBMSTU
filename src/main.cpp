
#include <QApplication>
#include <gui/CreateTasksWid.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
//    app.setFont( QFont("Calibri") );
    app.setFont( QFont("Arial") );

    CreateTasksWid wid;
    wid.show();

    return app.exec();
}
