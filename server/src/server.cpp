#include "../include/server.hpp"
#include <QTcpServer>
#include <QTcpSocket>

namespace Dandelion::Server {

    server::server(QObject *parent) {

    }

    bool server::start_server(qint16 port, std::string &connection_string) {
        m_Server = new QTcpServer(this);
        PORT = port;

        if (m_Server->listen(QHostAddress::Any, PORT)) {
            qDebug() << "Listening port" << PORT << "...";
            qDebug() << "My ip " << m_Server->serverAddress().toString();

            connect(
                    m_Server, SIGNAL(newConnection()), this, SLOT(incomingConnection())
            );

            if (connect_to_database(connection_string)) {
                qDebug() << "Connected to database";
            } else {
                qDebug() << "Couldn't connect to database";
                return false;
            }
        } else {
            qDebug() << "Couldn't start server";
            return false;
        }
        return true;
    }

    void server::disconnect_socket(client_socket *socket) {
        m_sockets.erase(m_sockets.find((socket->get_socket_id())));
        delete socket;
    }

    bool server::connect_to_database(std::string &connection_string) {
        static Database db(connection_string);
        m_db = &db;
        return db.m_connection.is_open();
    }

    void server::incomingConnection() {
        qDebug() << "New connection";
        QTcpSocket *new_connection = m_Server->nextPendingConnection();
        auto *new_socket = new client_socket(new_connection, m_db, this, new_connection->socketDescriptor());
        m_sockets[new_connection->socketDescriptor()] = new_socket;
    }

    server::~server() {
        for (auto socket: m_sockets) {
            socket->m_socket->deleteLater();
        }

        m_Server->deleteLater();
        delete m_db;
    }


//////////NEW


    client_socket::client_socket(QTcpSocket *socket, Database *db, server *server, quint16 id, QObject *parent)
            : m_socket(socket), m_db(db), m_server(server), m_socket_id(id),
              QObject(nullptr) {
        connect(m_socket, SIGNAL(disconnected()),
                this, SLOT(disconnect())
        );
        connect(m_socket, SIGNAL(readyRead()),
                this, SLOT(read_from_client())
        );
        m_request = new client_request(m_db);
    }


    client_socket::~client_socket() {
        m_socket->deleteLater();
    }

    void client_socket::send_to_client(const QByteArray &data) {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (quint16)data.size();
        out << data;
        m_socket->write(block);
       // m_socket->flush();
       m_socket->waitForBytesWritten();
    }

    void client_socket::read_from_client() {
        QByteArray data = m_socket->readAll();
        qDebug() << "got from client";
        QJsonDocument response = m_request->validate_request(data);
        qDebug() << "Before sending" << response.object().value("status").toString();
        qDebug() << "Before sending" << response.object().value("password").toString();
        qDebug() << "Before sending" << response.object().value("username").toString();
        qDebug() << "Before sending" << response.object().value("type").toString();
        send_to_client(response.toJson());
    }

    int client_socket::get_socket_id() {
        return m_socket_id;
    }

    void client_socket::disconnect() {
        m_server->disconnect_socket(this);
    }


//////////NEW
    client_request::client_request(Database *db) : m_db(db) {
    }

    QJsonDocument client_request::validate_request(const QByteArray &data) {
        QJsonParseError json_data_error{};
        QJsonDocument json_data = QJsonDocument::fromJson(data, &json_data_error);
        if (json_data_error.errorString().toInt() == QJsonParseError::NoError) {
            QString request_type = json_data.object().value("type").toString();
            if (!request_type.size()) {
                QJsonObject json_response;
                json_response["status"] = "error";
                json_response["text"] = "missing request type";
                QJsonDocument response(json_response);
                return response;
            }
            if (request_type == "signing_in") {
                QJsonDocument answer = sign_in(json_data.object());
                QJsonObject obj = answer.object();
                obj["type"] = "sign_in";
                answer.setObject(obj);
                return answer;
            } else if (request_type == "registration") {
                QJsonDocument answer =  make_registration(json_data.object());
                QJsonObject obj = answer.object();
                obj["type"] = "registration";
                answer.setObject(obj);
                return answer;
            } else {
                QJsonObject json_response;
                json_response["status"] = "error";
                json_response["text"] = "invalid request type";
                QJsonDocument response(json_response);
                return response;
            }
        }
        return json_data;
    }


    QJsonDocument client_request::sign_in(const QJsonObject &request) {
        QString received_login = request.value("username").toString();
        QString received_password = request.value("password").toString();
        User is_correct_user = User_Database::make_signing_in(received_login.toStdString(),
                                                              received_password.toStdString(), *m_db);
        return check_request(is_correct_user, "failed authorization");
    }


    QJsonDocument client_request::check_request(User user, const std::string &error_text) {
        QJsonObject response;
        if (user.m_username.empty() || user.m_password.empty()) {
            response["status"] = "error";
            response["text"] = QString::fromStdString(error_text);
            QJsonDocument server_response(response);
            return server_response;
        }
        response["status"] = "success";
        response["username"] = QString::fromStdString(user.m_username);
        response["password"] = QString::fromStdString(user.m_password);
        QJsonDocument server_response(response);
        return server_response;
    }

    QJsonDocument client_request::make_registration(const QJsonObject &request) {
        QString received_login = request.value("username").toString();
        QString received_password = request.value("password").toString();
        User probably_new_user = User_Database::create_user(received_login.toStdString(),
                                                            received_password.toStdString(), *m_db);
        return check_request(probably_new_user, "failed registration");
    }

}
