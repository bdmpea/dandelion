#include <utility>
#include <QDataStream>

#include "../include/client.h"

client::client(QObject *parent) {
    m_main_window = new MainWindow();
    connect(m_main_window, SIGNAL(signal_info_for_authorization()), this,
            SLOT(got_info_for_authorization()));
    connect(m_main_window, SIGNAL(signal_info_for_registration()), this,
            SLOT(got_info_for_registration()));
}


client::~client() noexcept {
    m_main_window->deleteLater();
}

void client::start() {
    m_main_window->show();
    m_client_socket = new socket_on_client(1338, "127.0.0.1");
    m_client_socket->connect_();
    connect(m_client_socket, SIGNAL(signal_info_for_registration_status(User_information)), this, SLOT(info_for_registration_status(User_information)));
    connect(m_client_socket, SIGNAL(signal_info_for_authorization_status(User_information)), this,
            SLOT(info_for_authorization_status(User_information)));
}

void client::got_info_for_registration() {
    qDebug() << "Got register data";
    m_client_socket->try_register_user(m_main_window->registration->get_username(),
                                       m_main_window->registration->get_password());
}

void client::got_info_for_authorization() {
    m_client_socket->try_sign_in_user(m_main_window->get_username(), m_main_window->get_password());
}

void client::info_for_authorization_status(User_information user) {
    m_user = std::move(m_client_socket->m_user);
    m_main_window->authorize(m_user.m_status);
}

void client::info_for_registration_status(User_information user) {
    m_user = std::move(m_client_socket->m_user);
    m_main_window->registration->register_user(m_user.m_status);
}

socket_on_client::socket_on_client(qint16 port, QString ip) : m_client_socket(new QTcpSocket(this)), m_PORT(port),
                                                              m_ip(std::move(ip)),
                                                              m_user("", "", "") {};

void socket_on_client::send_data_to_server(const QByteArray &data) {
    m_client_socket->write(data);
    m_client_socket->waitForBytesWritten();
}

void socket_on_client::connect_() {
    m_client_socket->connectToHost(m_ip, m_PORT);
    if (m_client_socket->waitForConnected()) {
        connect(m_client_socket, SIGNAL(readyRead()), this, SLOT(validate_server_answer()));
    }
}

socket_on_client::~socket_on_client() {
    m_client_socket->deleteLater();
}

void socket_on_client::validate_server_answer() {
    QDataStream in(&(*m_client_socket));
    quint16 block_size;
    in >> block_size;
    m_client_socket->read(sizeof(quint32));
    QByteArray server_response = m_client_socket->read(block_size);
    QJsonParseError error{};
    QJsonDocument answer = QJsonDocument::fromJson(server_response, &error);
    if (error.errorString().toInt() == QJsonParseError::NoError) {
        QString type = answer.object().value("type").toString();
        if (type == "sign_in") {
            check_sign_in(answer);
            emit signal_info_for_authorization_status(m_user);
        } else if (type == "registration") {
            check_register_user(answer);
            emit signal_info_for_registration_status(m_user);
        }
    } else {
        qDebug() << "Some errors with server data";
    }
}

void socket_on_client::try_sign_in_user(const QString &username, const QString &password) {
    QJsonObject request;
    request["type"] = "signing_in";
    request["username"] = username;
    request["password"] = password;
    QJsonDocument m_request(request);
    send_data_to_server(m_request.toJson());
}

void socket_on_client::try_register_user(const QString &username, const QString &password) {
    qDebug() << "Try register";
    QJsonObject request;
    request["type"] = "registration";
    request["username"] = username;
    request["password"] = password;
    QJsonDocument m_request(request);
    send_data_to_server(m_request.toJson());
}

void socket_on_client::check_sign_in(const QJsonDocument &answer) {
    qDebug() << "After sending" << answer.object().value("status").toString();
    qDebug() << "After sending" << answer.object().value("password").toString();
    qDebug() << "After sending" << answer.object().value("username").toString();
    if (answer.object().value("status").toString() == "success") {
        m_user = User_information(answer.object().value("username").toString(),
                                  answer.object().value("password").toString(),
                                  answer.object().value("status").toString());
        qDebug() << "user" << answer.object().value("status").toString();
        qDebug() << "user" << answer.object().value("password").toString();
        qDebug() << "user" << answer.object().value("username").toString();
    } else {
        m_user = User_information("", "", "error");
    }
}

void socket_on_client::check_register_user(const QJsonDocument &answer) {
    if (answer.object().value("status").toString() == "success") {
        m_user = User_information(answer.object().value("username").toString(),
                                  answer.object().value("password").toString(),
                                  answer.object().value("status").toString());
    } else {
        m_user = User_information("", "", "error");
    }
}
