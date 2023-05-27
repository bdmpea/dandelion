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
        static unsigned int is_used_login(const std::string &login, Database &db);                        //return id
        static User make_signing_in(const std::string &login, const std::string &password, Database &db);

        static User create_user(const std::string &login, const std::string &password, Database &db);

    };

    struct Dictionaries_Database {
        //TODO
    };


}  // namespace Dandelion::Server

#endif  // DANDELION_DATABASE_H
