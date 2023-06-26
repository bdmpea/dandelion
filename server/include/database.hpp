#ifndef DANDELION_DATABASE_H
#define DANDELION_DATABASE_H

#include <exception>
#include <fstream>
#include <iostream>
#include <mutex>
#include <pqxx/pqxx>
#include <string>
#include <thread>
#include <utility>
#include <vector>
#include <QVector>
#include <QString>

namespace Dandelion::Server {

    struct Database {
        std::mutex m_mutex;
        pqxx::connection m_connection;

        explicit Database(const std::string &connection_string);

        void do_query(const std::string &sql);

    private:
        friend struct User;
        friend struct Dictionaries;
    };

    struct User {
        std::string m_username;
        std::string m_password;
        User(std::string username, std::string password) : m_username(std::move(username)),
                                                           m_password(std::move(password)) {};
    };

    struct User_Database {
        static unsigned int is_used_login(const std::string &login, Database &db);
        static User make_signing_in(const std::string &login, const std::string &password, Database &db);
        static User create_user(const std::string &login, const std::string &password, Database &db);
        static void create_personal_dictionary(pqxx::work &worker, const std::string &login);
        static bool add_new_word(const std::string &login, const std::string &word, Database &db);
        static QVector<QVector<QString>> get_vocabulary(const std::string &login, Database &db);
    };

    struct Dictionaries_Database {
        //TODO
    };


}  // namespace Dandelion::Server

#endif  // DANDELION_DATABASE_H
