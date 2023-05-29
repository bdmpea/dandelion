#include <utility>

#include "../../server/include/client.h"

client::client(QObject *parent) {
//    m_main_window = new MainWindow();
//    connect(m_main_window, SIGNAL(signal_info_for_authorization()), this, SLOT(got_info_for_authorization()));
//    connect(m_main_window, SIGNAL(signal_info_for_registrtion()), this,
//            SLOT(got_info_for_registrtion()));
}



client::~client() noexcept {
  //  m_main_window->deleteLater();
}

void client::start() {
   // m_main_window->show();
    m_client_socket = new socket_on_client(1338, "127.0.0.1");
    m_client_socket->connect();
}

void client::got_info_for_registration() {

}

void client::got_info_for_authorization() {

}


socket_on_client::socket_on_client(qint16 port, QString ip) : m_client_socket(new QTcpSocket(this)), m_PORT(port),
                                                              m_ip(std::move(ip)),
                                                              m_user("", "") {};

void socket_on_client::send_data_to_server(const QByteArray &data) {
    m_client_socket->write(data);
    m_client_socket->waitForBytesWritten();
}

void socket_on_client::connect() {
    m_client_socket->connectToHost(m_ip, m_PORT);
    if (m_client_socket->waitForConnected()) {
        //TODO: error
    }
}

socket_on_client::~socket_on_client() {
    m_client_socket->deleteLater();
}

bool socket_on_client::sign_in_user(const QString &username, const QString &password) {
    QJsonObject request;
    request["type"] = "signing_in";
    request["username"] = username;
    request["password"] = password;
    QJsonDocument m_request(request);
    send_data_to_server(m_request.toJson());
    QDataStream in(&(*m_client_socket));
    quint16 block_size;
    in >> block_size;
    m_client_socket->read(sizeof(quint32));
    QByteArray server_response = m_client_socket->read(block_size);
   // QByteArray server_response = m_client_socket->readAll();
    QJsonParseError error{};
    QJsonDocument answer = QJsonDocument::fromJson(server_response, &error);
    qDebug() << answer.object().value("password").toString();
    if (answer.object().value("status").toString() == "success") {
        m_user = User_information(answer.object().value("username").toString(),
                                  answer.object().value("password").toString());
        return true;
    } else {
        return false;
    }
}

bool socket_on_client::register_user(const QString &username, const QString &password) {
    QJsonObject request;
    request["type"] = "registration";
    request["username"] = username;
    request["password"] = password;
    QJsonDocument m_request(request);
    send_data_to_server(m_request.toJson());
    m_client_socket->waitForReadyRead();
    QByteArray server_response = m_client_socket->readAll();
    QJsonParseError error;
    QJsonDocument answer = QJsonDocument::fromJson(server_response, &error);
    if (answer.object().value("status").toString() == "success") {
        m_user = User_information(answer.object().value("username").toString(),
                                  answer.object().value("password").toString());
        return true;
    } else {
        return false;
    }
}