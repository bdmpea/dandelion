#include "../include/database.hpp"

namespace Dandelion::Server {
    Database::Database(const std::string &connection_string) : m_connection(connection_string.c_str()) {}

    unsigned int User_Database::is_used_login(const std::string &login, Database &db) {
        pqxx::work worker(db.m_connection);
        int user_id;
        try {
            user_id = worker.query_value<int>("SELECT id from register_users WHERE username='" + login + "'");
            //TODO
        }
        catch (...) {
            return 0;
        }
        return user_id;
    }

    User User_Database::create_user(const std::string &login, const std::string &password, Database &db) {
        try {
            if (!is_used_login(login, db)) {
                db.Database::do_query("INSERT INTO register_users"\
            "(username, password)"\
            "VALUES(\'" + login + "\',\'" + password + "\');"
                );
                return {login, password};
            }
        } catch (...) {
            return {"", ""};
        }
        return {"", ""};
    }

    void Database::do_query(const std::string &query) {
        pqxx::work worker(m_connection);
        worker.exec(query);
        worker.commit();
    }

    User User_Database::make_signing_in(const std::string &login, const std::string &password, Database &db) {
        unsigned int user_id = is_used_login(login, db);
        if (user_id) {
            pqxx::work worker(db.m_connection);
            auto correct_password = worker.query_value<std::string>(
                    "SELECT password FROM register_users WHERE username ='" + login + "\'");
            if (password == correct_password) {
                return {login, password};
            } else {
                return {"", ""};
            }
        }
        return {"", ""};
    }
}