#include <utility>
#include <QDataStream>

#include "../include/client.h"

client::client(QObject *parent) {
    m_main_window = new MainWindow();
    connect(m_main_window, SIGNAL(signal_info_for_authorization()),
            this,
            SLOT(got_info_for_authorization()));
    connect(m_main_window, SIGNAL(signal_info_for_registration()),
            this,
            SLOT(got_info_for_registration()));
    connect(m_main_window, SIGNAL(signal_info_for_new_word()),
            this,
            SLOT(got_info_for_new_word()));
    connect(m_main_window,
            SIGNAL(signal_info_for_new_word_cards()),
            this,
            SLOT(got_info_for_new_word_cards()));
    connect(m_main_window,
            SIGNAL(signal_for_new_words_from_vocabulary()),
            this,
            SLOT(get_vocabulary()));
}


client::~client() noexcept {
    m_main_window->deleteLater();
}

void client::start() {
    m_main_window->show();
    m_client_socket = new socket_on_client(1338, "127.0.0.1");
    m_client_socket->connect_();
    connect(m_client_socket,
            SIGNAL(signal_info_for_registration_status(User_information)),
            this,
            SLOT(info_for_registration_status(User_information)));
    connect(m_client_socket,
            SIGNAL(signal_info_for_authorization_status(User_information)),
            this,
            SLOT(info_for_authorization_status(User_information)));
    connect(m_client_socket,
            SIGNAL(signal_info_for_vocabulary_status(QVector<QVector<QString> >)),
            this,
            SLOT(info_info_for_vocabulary_status(QVector<QVector<QString> >))
            );
}

void client::got_info_for_registration() {
    qDebug() << "Got register data";
    m_client_socket->try_register_user(m_main_window->registration->get_username(),
                                       m_main_window->registration->get_password());
}

void client::got_info_for_authorization() {
    m_client_socket->try_sign_in_user(m_main_window->get_username(), m_main_window->get_password());
}

void client::got_info_for_new_word() {
    qDebug() << "Got new word";
    m_client_socket->try_add_new_word(m_user.m_username, m_main_window->window->get_new_word());
}

void client::got_info_for_new_word_cards() {
    qDebug() << "Got new word from cards";
    m_client_socket->try_add_new_word(m_user.m_username, m_main_window->window->personal_account->get_new_word());
}

void client::get_vocabulary() {
    qDebug() << "Need vocabulary";
    m_client_socket->try_get_vocabulary(m_user.m_username);
}

void client::info_for_authorization_status(User_information user) {
    m_user = std::move(m_client_socket->m_user);
    m_main_window->authorize(m_user.m_status);
}

void client::info_for_registration_status(User_information user) {
    m_user = std::move(m_client_socket->m_user);
    m_main_window->registration->register_user(m_user.m_status);
}

void client::info_info_for_vocabulary_status(QVector<QVector<QString>> vocabulary) {
    m_main_window->window->personal_account->vocabulary->display(vocabulary);
    m_main_window->window->personal_account->vocabulary->show();
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
        } else if (type == "new_word") {
            //TODO
        } else if(type == "get_vocabulary"){
            QVector<QVector<QString>> vocabulary = unpack_vocabulary(answer);
            emit signal_info_for_vocabulary_status(vocabulary);
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

void socket_on_client::try_add_new_word(const QString &username, const QString &word) {
    qDebug() << "Try add word";
    QJsonObject request;
    request["type"] = "new_word";
    request["username"] = username;
    request["word"] = word;
    QJsonDocument m_request(request);
    send_data_to_server(m_request.toJson());
}

void socket_on_client::try_get_vocabulary(const QString &username) {
    qDebug() << "Try get vocabulary";
    QJsonObject request;
    request["type"] = "get_vocabulary";
    request["username"] = username;
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
QVector<QVector<QString>> socket_on_client::unpack_vocabulary(const QJsonDocument &answer) {
    QVector<QVector<QString>> vocabulary;
      int size = std::stoi(answer.object().value("size").toString().toStdString());
      vocabulary.resize(size);
      for(int i = 0; i < size; i++){
          vocabulary[i].push_back(answer.object().value("word"+QString::fromStdString(std::to_string(i))).toString());
          vocabulary[i].push_back(answer.object().value("meaning"+QString::fromStdString(std::to_string(i))).toString());
      }
    return vocabulary;
}