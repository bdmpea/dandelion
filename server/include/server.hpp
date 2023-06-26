#ifndef SERVER_H
#define SERVER_H

#include <QMessageBox>
#include <QTcpServer>
#include <QWidget>
#include <QJsonDocument>
#include <QJsonObject>
#include "database.hpp"
#include <QMap>

class QTcpServer;

class QTcpSocket;

struct client_socket;

namespace Dandelion::Server {
    class client_socket;

    class server : public QObject {
    Q_OBJECT
    private:
        QByteArray Data;
        qint16 PORT{};
        Database *m_db{};
        QMap<qint64, client_socket *> m_sockets;
    public:
        QTcpServer *m_Server{};

        ~server() override;

        bool start_server(qint16 port, std::string &config_file);

        bool connect_to_database(std::string &config_file);

        server(QObject *parent = nullptr);

        static server &getInstance() {
            static server m_Server;
            return m_Server;
        };

        void disconnect_socket(client_socket *socket);

    public slots:

        void incomingConnection();
    };


//////////NEW


    struct client_request;

    class client_socket : public QObject {
    Q_OBJECT

    private:
        Database *m_db;
        server *m_server;
        quint16 m_socket_id;
        client_request *m_request;
    public:
        QTcpSocket *m_socket;

        ~client_socket() override;

        void send_to_client(const QByteArray &data);

        explicit client_socket(QTcpSocket *socket, Database *db, server *server, quint16 id, QObject *parent = nullptr);

        int get_socket_id();

    public slots:

        void read_from_client();

        void disconnect();

    };


//////////NEW


    class client_request : public QObject {
    Q_OBJECT
        Database *m_db;
        client_socket *request_sender;
    public:
        explicit client_request(Database *db);

        QJsonDocument check_request(User probably_new_user, const std::string &error_text);

        QJsonDocument validate_request(const QByteArray &data);

        QJsonDocument validate_request_1(const QByteArray &data);

        QJsonDocument make_registration(const QJsonObject &request);

        QJsonDocument sign_in(const QJsonObject &request);

        QJsonDocument add_new_word(const QJsonObject &request);

        QJsonDocument get_words_from_vocabulary(const QJsonObject &request);
    };


}

#endif // SERVER_H
