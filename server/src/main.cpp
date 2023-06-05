#include <QCoreApplication>

#include "../include/server.hpp"
#include "../../cllient/include/client.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    qint16 port = 1338;
    std::string connection_string = "host=127.0.0.1 port=5432 dbname=p34096 user=postgres password = Grisha09012004";
    if(!Dandelion::Server::server::getInstance().start_server(port, connection_string)) {
        return 0;
    }
    return a.exec();
}