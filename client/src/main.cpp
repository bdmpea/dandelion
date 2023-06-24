#include "client.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR","1");
    QApplication a(argc, argv);
    client new_client;
    new_client.start();
    return a.exec();
}
