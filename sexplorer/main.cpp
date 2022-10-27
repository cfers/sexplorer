#include "sexplorer.h"
#include <QtWidgets/QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    sexplorer w;
    w.show();
    return a.exec();
}
