#include "../include/main_window.h"
#include "../../server/include/server.hpp"
#include "../include/client.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR","1");
    QApplication a(argc, argv);
    qint16 port = 1338;
    std::string connection_string = "host=127.0.0.1 port=5432 dbname=p34096 user=postgres password = Grisha09012004";
    if(!Dandelion::Server::server::getInstance().start_server(port, connection_string)) {
        return 0;
    }
    client first_client;
    first_client.start();
    client second_client;
    second_client.start();
    return a.exec();
}
