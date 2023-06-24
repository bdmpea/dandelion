#ifndef FIRST_PROJECT_CLIENT_H
#define FIRST_PROJECT_CLIENT_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QTcpSocket>
#include "main_window.h"

class User_information {
    QString m_username;
    QString m_password;
public:
    QString m_status;
    User_information() : m_username(""), m_password(""), m_status("") {};

    User_information(const QString &username, const QString &password, const QString &status) : m_username(username), m_password(password), m_status(status) {};
};


class socket_on_client;


class client : public QObject {

Q_OBJECT
    QString m_username;
    QString m_password;
    User_information m_user;

public:
    MainWindow *m_main_window;
    socket_on_client *m_client_socket;
    client(QObject *parent = nullptr);

    virtual ~client() ;

    void start();

    void send_data(const QString &email, const QString &password);

public slots:
    void got_info_for_registration();
    void got_info_for_authorization();
    void info_for_registration_status(User_information user);
    void info_for_authorization_status(User_information user);
signals:
    void signal_info_for_registration();
    void signal_info_for_authorization();
};

class socket_on_client : public QObject {

Q_OBJECT
    QTcpSocket *m_client_socket;
    quint16 m_PORT;
    QString m_ip;
public:
    User_information m_user;

    socket_on_client(qint16 port, QString ip);

    ~socket_on_client() override;

    void connect_();

    void send_data_to_server(const QByteArray &data);

    void try_sign_in_user(const QString &username, const QString &password);

    void check_sign_in(const QJsonDocument &answer);

    void check_register_user(const QJsonDocument &answer);

    void try_register_user(const QString &username, const QString &password);
public  slots:
     void validate_server_answer();
signals:
    void signal_info_for_registration_status(User_information user);
    void signal_info_for_authorization_status(User_information user);
};


#endif //FIRST_PROJECT_CLIENT_H
