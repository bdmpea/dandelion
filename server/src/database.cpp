#include "../include/database.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QTcpSocket>
#include <QVector>

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
                //  create_personal_dictionary(db, login);
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
        //  qDebug() << user_id;
        if (user_id) {
            pqxx::work worker(db.m_connection);
            auto correct_password = worker.query_value<std::string>(
                    "SELECT password FROM register_users WHERE username ='" + login + "\'");
            // qDebug() << QString::fromStdString(correct_password);
            if (password == correct_password) {
                create_personal_dictionary(worker, login);
                return {login, password};
            } else {
                return {"", ""};
            }
        }
        return {"", ""};
    }

    bool User_Database::add_new_word(const std::string &login, const std::string &word, Database &db) {
        try {
            db.Database::do_query("INSERT INTO " + login + "(word, meaning, examples) VALUES"\
            "(\'" + word + "\' , 'some meaning', 'some example')"
            );
        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            return false;
        }
        return true;
    }

    void User_Database::create_personal_dictionary(pqxx::work &worker, const std::string &login) {
        try {
            worker.exec("CREATE TABLE IF NOT EXISTS " + login + " ( "\
         " id serial  PRIMARY KEY,"\
         " word character varying(32) NOT NULL,"\
         " meaning character varying(64) NOT NULL,"\
         " examples  character varying(64) NOT NULL );"
            );
            // worker.commit();
            bool if_empty = worker.query_value<bool>(" SELECT(SELECT count(*) FROM " + login + ") = 0 ");
            qDebug() << if_empty;
            if (if_empty) {
                worker.exec("INSERT INTO " + login + "(word, meaning, examples) VALUES"\
            "('cat', 'a small  carnivorous mammal with soft fur', 'black cat'),"\
            "('dog', 'a  carnivorous mammal that  has a long snout', 'white dog'),"\
            "('commit', 'perform an act with a negative connotation', 'make new commit'),"\
            "('issue', 'some situation or event that is thought about', 'hard issue'),"\
            "('stock', 'capital raised by a corporation ', 'apple stock is cheap'),"\
            "('circumstances', 'one overall condition in life', 'the circumstances leading up'),"\
            "('level', 'a relative position or degree of value ', 'rates reach the same level'),"\
            "('affect', 'have an influence upon', 'companies affected by the flooding'),"\
            "('institute', 'set up or lay the groundwork for', 'focused on instituting higher labor'),"\
            "('render', 'give an interpretation of', 'but authorities had rendered the weapon')"
                );
            }
            worker.commit();
        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }

    QVector<QVector<QString>> User_Database::get_vocabulary(const std::string &login, Database &db) {
        QVector<QVector<QString>> vocabulary;
        try{
            pqxx::work worker(db.m_connection);
            pqxx::result result = worker.exec("SELECT * FROM " + login);
            vocabulary.resize(result.size());
            for(int i = 0; i < result.size(); i++){
                vocabulary[i].push_back(QString::fromStdString(to_string(result[i][1])));
                vocabulary[i].push_back(QString::fromStdString(to_string(result[i][2])));
            }
        } catch(const std::exception &e){
            std::cerr << e.what() << std::endl;
        }
        return vocabulary;
    }
}

