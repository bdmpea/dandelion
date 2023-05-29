#ifndef FIRST_PROJECT_CLIENT_H
#define FIRST_PROJECT_CLIENT_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QTcpSocket>
#include "../../cllient/include/mainwindow.h"

class User_information {
    QString m_username;
    QString m_password;
public:
    User_information() : m_username(""), m_password("") {};

    User_information(const QString &username, const QString &password) : m_username(username), m_password(password) {};
};


class socket_on_client;


class client : public QObject {

Q_OBJECT
    QString m_username;
    QString m_password;
    User_information user;
   // MainWindow *m_main_window;
public:
    socket_on_client *m_client_socket;
    client(QObject *parent = nullptr);

    ~client() override;

    void start();

    void send_data(const QString &email, const QString &password);

public slots:
    void got_info_for_registration();
    void got_info_for_authorization();
signals:
    void signal_info_for_registration();
    void signal_info_for_authorization();
};

class socket_on_client : public QObject {

Q_OBJECT
    QTcpSocket *m_client_socket;
    quint16 m_PORT;
    QString m_ip;
    User_information m_user;
public:
    socket_on_client(qint16 port, QString ip);

    ~socket_on_client();

    void connect();

    void send_data_to_server(const QByteArray &data);

    bool sign_in_user(const QString &username, const QString &password);

    bool register_user(const QString &username, const QString &password);
};


#endif //FIRST_PROJECT_CLIENT_H
