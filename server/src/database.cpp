#include "../include/database.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QTcpSocket>
#include <QVector>
#include <QString>

namespace Dandelion::Server {
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
        qDebug() << user_id;
        if (user_id) {
            pqxx::work worker(db.m_connection);
            auto correct_password = worker.query_value<std::string>(
                    "SELECT password FROM register_users WHERE username ='" + login + "\'");

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
        try {
            pqxx::work worker(db.m_connection);
            pqxx::result result = worker.exec("SELECT * FROM " + login);
            vocabulary.resize(result.size());
            for (int i = 0; i < result.size(); i++) {
                vocabulary[i].push_back(QString::fromStdString(to_string(result[i][1])));
                vocabulary[i].push_back(QString::fromStdString(to_string(result[i][2])));
            }
        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
        return vocabulary;
    }


    Database::Database(const std::string &connection_string) : m_connection(connection_string.c_str()) {
        do_query(R"sql(CREATE TABLE IF NOT EXISTS  huge_vocabulary (
	word TEXT,
	meaning TEXT,
	examples TEXT
);
insert into huge_vocabulary (word, meaning, examples) values ('ultrices', 'Nunc nisl. Duis bibendum, felis sed interdum venenatis, turpis enim blandit mi, in porttitor pede justo eu massa. Donec dapibus. Duis at velit eu est congue elementum.', 'Donec odio justo, sollicitudin ut, suscipit a, feugiat et, eros.');
insert into huge_vocabulary (word, meaning, examples) values ('blandit', 'Etiam faucibus cursus urna. Ut tellus. Nulla ut erat id mauris vulputate elementum. Nullam varius.', 'Mauris sit amet eros.');
insert into huge_vocabulary (word, meaning, examples) values ('dui', 'Donec odio justo, sollicitudin ut, suscipit a, feugiat et, eros. Vestibulum ac est lacinia nisi venenatis tristique. Fusce congue, diam id ornare imperdiet, sapien urna pretium nisl, ut volutpat sapien arcu sed augue. Aliquam erat volutpat.', 'Aenean auctor gravida sem.');
insert into huge_vocabulary (word, meaning, examples) values ('neque', 'Suspendisse potenti. In eleifend quam a odio. In hac habitasse platea dictumst. Maecenas ut massa quis augue luctus tincidunt.', 'Sed vel enim sit amet nunc viverra dapibus.');
insert into huge_vocabulary (word, meaning, examples) values ('consequat', 'Nunc rhoncus dui vel sem. Sed sagittis. Nam congue, risus semper porta volutpat, quam pede lobortis ligula, sit amet eleifend pede libero quis orci. Nullam molestie nibh in lectus.', 'Praesent id massa id nisl venenatis lacinia.');
insert into huge_vocabulary (word, meaning, examples) values ('pellentesque', 'Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Etiam vel augue. Vestibulum rutrum rutrum neque. Aenean auctor gravida sem.', 'Maecenas rhoncus aliquam lacus.');
insert into huge_vocabulary (word, meaning, examples) values ('risus', 'Phasellus sit amet erat. Nulla tempus. Vivamus in felis eu sapien cursus vestibulum. Proin eu mi.', 'Praesent lectus.');
insert into huge_vocabulary (word, meaning, examples) values ('sapien', 'In blandit ultrices enim. Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Proin interdum mauris non ligula pellentesque ultrices. Phasellus id sapien in sapien iaculis congue.', 'Nulla tellus.');
insert into huge_vocabulary (word, meaning, examples) values ('purus', 'Quisque erat eros, viverra eget, congue eget, semper rutrum, nulla. Nunc purus. Phasellus in felis. Donec semper sapien a libero.', 'In eleifend quam a odio.');
insert into huge_vocabulary (word, meaning, examples) values ('erat', 'Morbi a ipsum. Integer a nibh. In quis justo. Maecenas rhoncus aliquam lacus.', 'Suspendisse accumsan tortor quis turpis.');
insert into huge_vocabulary (word, meaning, examples) values ('in', 'Cras pellentesque volutpat dui. Maecenas tristique, est et tempus semper, est quam pharetra magna, ac consequat metus sapien ut nunc. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Mauris viverra diam vitae quam. Suspendisse potenti.', 'Mauris ullamcorper purus sit amet nulla.');
insert into huge_vocabulary (word, meaning, examples) values ('molestie', 'Mauris enim leo, rhoncus sed, vestibulum sit amet, cursus id, turpis. Integer aliquet, massa id lobortis convallis, tortor risus dapibus augue, vel accumsan tellus nisi eu orci. Mauris lacinia sapien quis libero. Nullam sit amet turpis elementum ligula vehicula consequat.', 'Donec ut dolor.');
insert into huge_vocabulary (word, meaning, examples) values ('vestibulum', 'Vestibulum ac est lacinia nisi venenatis tristique. Fusce congue, diam id ornare imperdiet, sapien urna pretium nisl, ut volutpat sapien arcu sed augue. Aliquam erat volutpat. In congue.', 'In blandit ultrices enim.');
insert into huge_vocabulary (word, meaning, examples) values ('sed', 'Curabitur at ipsum ac tellus semper interdum. Mauris ullamcorper purus sit amet nulla. Quisque arcu libero, rutrum ac, lobortis vel, dapibus at, diam. Nam tristique tortor eu pede.', 'Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus.');
insert into huge_vocabulary (word, meaning, examples) values ('sed', 'Morbi vel lectus in quam fringilla rhoncus. Mauris enim leo, rhoncus sed, vestibulum sit amet, cursus id, turpis. Integer aliquet, massa id lobortis convallis, tortor risus dapibus augue, vel accumsan tellus nisi eu orci. Mauris lacinia sapien quis libero.', 'Ut at dolor quis odio consequat varius.');
insert into huge_vocabulary (word, meaning, examples) values ('donec', 'Integer tincidunt ante vel ipsum. Praesent blandit lacinia erat. Vestibulum sed magna at nunc commodo placerat. Praesent blandit.', 'Ut tellus.');
insert into huge_vocabulary (word, meaning, examples) values ('ultrices', 'In hac habitasse platea dictumst. Maecenas ut massa quis augue luctus tincidunt. Nulla mollis molestie lorem. Quisque ut erat.', 'Nulla tempus.');
insert into huge_vocabulary (word, meaning, examples) values ('pulvinar', 'Vivamus vestibulum sagittis sapien. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Etiam vel augue. Vestibulum rutrum rutrum neque.', 'Donec quis orci eget orci vehicula condimentum.');
insert into huge_vocabulary (word, meaning, examples) values ('id', 'Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Duis faucibus accumsan odio. Curabitur convallis. Duis consequat dui nec nisi volutpat eleifend. Donec ut dolor.', 'Integer ac neque.');
insert into huge_vocabulary (word, meaning, examples) values ('auctor', 'Aenean lectus. Pellentesque eget nunc. Donec quis orci eget orci vehicula condimentum. Curabitur in libero ut massa volutpat convallis.', 'Morbi odio odio, elementum eu, interdum eu, tincidunt in, leo.');
insert into huge_vocabulary (word, meaning, examples) values ('eros', 'Aliquam sit amet diam in magna bibendum imperdiet. Nullam orci pede, venenatis non, sodales sed, tincidunt eu, felis. Fusce posuere felis sed lacus. Morbi sem mauris, laoreet ut, rhoncus aliquet, pulvinar sed, nisl.', 'Cras mi pede, malesuada in, imperdiet et, commodo vulputate, justo.');
insert into huge_vocabulary (word, meaning, examples) values ('justo', 'Maecenas ut massa quis augue luctus tincidunt. Nulla mollis molestie lorem. Quisque ut erat. Curabitur gravida nisi at nibh.', 'Etiam faucibus cursus urna.');
insert into huge_vocabulary (word, meaning, examples) values ('lacinia', 'Phasellus id sapien in sapien iaculis congue. Vivamus metus arcu, adipiscing molestie, hendrerit at, vulputate vitae, nisl. Aenean lectus. Pellentesque eget nunc.', 'Aliquam augue quam, sollicitudin vitae, consectetuer eget, rutrum at, lorem.');
insert into huge_vocabulary (word, meaning, examples) values ('nulla', 'Aliquam augue quam, sollicitudin vitae, consectetuer eget, rutrum at, lorem. Integer tincidunt ante vel ipsum. Praesent blandit lacinia erat. Vestibulum sed magna at nunc commodo placerat.', 'In quis justo.');
insert into huge_vocabulary (word, meaning, examples) values ('augue', 'Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Donec pharetra, magna vestibulum aliquet ultrices, erat tortor sollicitudin mi, sit amet lobortis sapien sapien non mi. Integer ac neque. Duis bibendum. Morbi non quam nec dui luctus rutrum.', 'Donec dapibus.');
insert into huge_vocabulary (word, meaning, examples) values ('sapien', 'Duis aliquam convallis nunc. Proin at turpis a pede posuere nonummy. Integer non velit. Donec diam neque, vestibulum eget, vulputate ut, ultrices vel, augue.', 'Nam nulla.');
insert into huge_vocabulary (word, meaning, examples) values ('non', 'Integer ac leo. Pellentesque ultrices mattis odio. Donec vitae nisi. Nam ultrices, libero non mattis pulvinar, nulla pede ullamcorper augue, a suscipit nulla elit ac nulla.', 'Proin at turpis a pede posuere nonummy.');
insert into huge_vocabulary (word, meaning, examples) values ('sit', 'Quisque porta volutpat erat. Quisque erat eros, viverra eget, congue eget, semper rutrum, nulla. Nunc purus. Phasellus in felis.', 'Pellentesque at nulla.');
insert into huge_vocabulary (word, meaning, examples) values ('vestibulum', 'In hac habitasse platea dictumst. Aliquam augue quam, sollicitudin vitae, consectetuer eget, rutrum at, lorem. Integer tincidunt ante vel ipsum. Praesent blandit lacinia erat.', 'Fusce consequat.');
insert into huge_vocabulary (word, meaning, examples) values ('metus', 'Nulla neque libero, convallis eget, eleifend luctus, ultricies eu, nibh. Quisque id justo sit amet sapien dignissim vestibulum. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Nulla dapibus dolor vel est. Donec odio justo, sollicitudin ut, suscipit a, feugiat et, eros.', 'Nullam varius.');
insert into huge_vocabulary (word, meaning, examples) values ('lobortis', 'Vestibulum sed magna at nunc commodo placerat. Praesent blandit. Nam nulla. Integer pede justo, lacinia eget, tincidunt eget, tempus vel, pede.', 'Nulla nisl.');
insert into huge_vocabulary (word, meaning, examples) values ('a', 'In hac habitasse platea dictumst. Maecenas ut massa quis augue luctus tincidunt. Nulla mollis molestie lorem. Quisque ut erat.', 'Morbi vestibulum, velit id pretium iaculis, diam erat fermentum justo, nec condimentum neque sapien placerat ante.');
insert into huge_vocabulary (word, meaning, examples) values ('eget', 'Curabitur gravida nisi at nibh. In hac habitasse platea dictumst. Aliquam augue quam, sollicitudin vitae, consectetuer eget, rutrum at, lorem. Integer tincidunt ante vel ipsum.', 'Donec posuere metus vitae ipsum.');
insert into huge_vocabulary (word, meaning, examples) values ('consequat', 'Quisque erat eros, viverra eget, congue eget, semper rutrum, nulla. Nunc purus. Phasellus in felis. Donec semper sapien a libero.', 'Vestibulum ac est lacinia nisi venenatis tristique.');
insert into huge_vocabulary (word, meaning, examples) values ('a', 'Quisque erat eros, viverra eget, congue eget, semper rutrum, nulla. Nunc purus. Phasellus in felis. Donec semper sapien a libero.', 'Duis aliquam convallis nunc.');
insert into huge_vocabulary (word, meaning, examples) values ('eleifend', 'In quis justo. Maecenas rhoncus aliquam lacus. Morbi quis tortor id nulla ultrices aliquet. Maecenas leo odio, condimentum id, luctus nec, molestie sed, justo.', 'Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus.');
insert into huge_vocabulary (word, meaning, examples) values ('tortor', 'Nullam sit amet turpis elementum ligula vehicula consequat. Morbi a ipsum. Integer a nibh. In quis justo.', 'Nulla justo.');
insert into huge_vocabulary (word, meaning, examples) values ('consequat', 'Maecenas leo odio, condimentum id, luctus nec, molestie sed, justo. Pellentesque viverra pede ac diam. Cras pellentesque volutpat dui. Maecenas tristique, est et tempus semper, est quam pharetra magna, ac consequat metus sapien ut nunc.', 'Donec dapibus.');
insert into huge_vocabulary (word, meaning, examples) values ('morbi', 'Fusce posuere felis sed lacus. Morbi sem mauris, laoreet ut, rhoncus aliquet, pulvinar sed, nisl. Nunc rhoncus dui vel sem. Sed sagittis.', 'Cras mi pede, malesuada in, imperdiet et, commodo vulputate, justo.');
insert into huge_vocabulary (word, meaning, examples) values ('justo', 'Morbi non quam nec dui luctus rutrum. Nulla tellus. In sagittis dui vel nisl. Duis ac nibh.', 'Vestibulum ac est lacinia nisi venenatis tristique.');
insert into huge_vocabulary (word, meaning, examples) values ('orci', 'Fusce lacus purus, aliquet at, feugiat non, pretium quis, lectus. Suspendisse potenti. In eleifend quam a odio. In hac habitasse platea dictumst.', 'Duis ac nibh.');
insert into huge_vocabulary (word, meaning, examples) values ('faucibus', 'Morbi ut odio. Cras mi pede, malesuada in, imperdiet et, commodo vulputate, justo. In blandit ultrices enim. Lorem ipsum dolor sit amet, consectetuer adipiscing elit.', 'In hac habitasse platea dictumst.');
insert into huge_vocabulary (word, meaning, examples) values ('eleifend', 'Vivamus in felis eu sapien cursus vestibulum. Proin eu mi. Nulla ac enim. In tempor, turpis nec euismod scelerisque, quam turpis adipiscing lorem, vitae mattis nibh ligula nec sem.', 'Etiam vel augue.');
insert into huge_vocabulary (word, meaning, examples) values ('quis', 'Nunc purus. Phasellus in felis. Donec semper sapien a libero. Nam dui.', 'In eleifend quam a odio.');
insert into huge_vocabulary (word, meaning, examples) values ('morbi', 'Quisque porta volutpat erat. Quisque erat eros, viverra eget, congue eget, semper rutrum, nulla. Nunc purus. Phasellus in felis.', 'Duis ac nibh.');
insert into huge_vocabulary (word, meaning, examples) values ('ante', 'Praesent lectus. Vestibulum quam sapien, varius ut, blandit non, interdum in, ante. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Duis faucibus accumsan odio. Curabitur convallis.', 'Maecenas ut massa quis augue luctus tincidunt.');
insert into huge_vocabulary (word, meaning, examples) values ('et', 'Morbi odio odio, elementum eu, interdum eu, tincidunt in, leo. Maecenas pulvinar lobortis est. Phasellus sit amet erat. Nulla tempus.', 'Vestibulum ac est lacinia nisi venenatis tristique.');
insert into huge_vocabulary (word, meaning, examples) values ('pede', 'Aliquam sit amet diam in magna bibendum imperdiet. Nullam orci pede, venenatis non, sodales sed, tincidunt eu, felis. Fusce posuere felis sed lacus. Morbi sem mauris, laoreet ut, rhoncus aliquet, pulvinar sed, nisl.', 'Aenean lectus.');
insert into huge_vocabulary (word, meaning, examples) values ('eget', 'Nam ultrices, libero non mattis pulvinar, nulla pede ullamcorper augue, a suscipit nulla elit ac nulla. Sed vel enim sit amet nunc viverra dapibus. Nulla suscipit ligula in lacus. Curabitur at ipsum ac tellus semper interdum.', 'Nullam porttitor lacus at turpis.');
insert into huge_vocabulary (word, meaning, examples) values ('viverra', 'Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Donec pharetra, magna vestibulum aliquet ultrices, erat tortor sollicitudin mi, sit amet lobortis sapien sapien non mi. Integer ac neque. Duis bibendum. Morbi non quam nec dui luctus rutrum.', 'Morbi odio odio, elementum eu, interdum eu, tincidunt in, leo.');
insert into huge_vocabulary (word, meaning, examples) values ('sit', 'Aliquam augue quam, sollicitudin vitae, consectetuer eget, rutrum at, lorem. Integer tincidunt ante vel ipsum. Praesent blandit lacinia erat. Vestibulum sed magna at nunc commodo placerat.', 'Aenean lectus.');
insert into huge_vocabulary (word, meaning, examples) values ('pede', 'Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Duis faucibus accumsan odio. Curabitur convallis. Duis consequat dui nec nisi volutpat eleifend. Donec ut dolor.', 'Vivamus vestibulum sagittis sapien.');
insert into huge_vocabulary (word, meaning, examples) values ('lacus', 'Mauris sit amet eros. Suspendisse accumsan tortor quis turpis. Sed ante. Vivamus tortor.', 'Sed accumsan felis.');
insert into huge_vocabulary (word, meaning, examples) values ('ante', 'Vestibulum ac est lacinia nisi venenatis tristique. Fusce congue, diam id ornare imperdiet, sapien urna pretium nisl, ut volutpat sapien arcu sed augue. Aliquam erat volutpat. In congue.', 'Vestibulum rutrum rutrum neque.');
insert into huge_vocabulary (word, meaning, examples) values ('amet', 'In hac habitasse platea dictumst. Morbi vestibulum, velit id pretium iaculis, diam erat fermentum justo, nec condimentum neque sapien placerat ante. Nulla justo. Aliquam quis turpis eget elit sodales scelerisque.', 'Mauris enim leo, rhoncus sed, vestibulum sit amet, cursus id, turpis.');
insert into huge_vocabulary (word, meaning, examples) values ('sit', 'Vestibulum ac est lacinia nisi venenatis tristique. Fusce congue, diam id ornare imperdiet, sapien urna pretium nisl, ut volutpat sapien arcu sed augue. Aliquam erat volutpat. In congue.', 'Integer pede justo, lacinia eget, tincidunt eget, tempus vel, pede.');
insert into huge_vocabulary (word, meaning, examples) values ('ac', 'Proin eu mi. Nulla ac enim. In tempor, turpis nec euismod scelerisque, quam turpis adipiscing lorem, vitae mattis nibh ligula nec sem. Duis aliquam convallis nunc.', 'Mauris ullamcorper purus sit amet nulla.');
insert into huge_vocabulary (word, meaning, examples) values ('nulla', 'Nam congue, risus semper porta volutpat, quam pede lobortis ligula, sit amet eleifend pede libero quis orci. Nullam molestie nibh in lectus. Pellentesque at nulla. Suspendisse potenti.', 'Aenean auctor gravida sem.');
insert into huge_vocabulary (word, meaning, examples) values ('scelerisque', 'Maecenas leo odio, condimentum id, luctus nec, molestie sed, justo. Pellentesque viverra pede ac diam. Cras pellentesque volutpat dui. Maecenas tristique, est et tempus semper, est quam pharetra magna, ac consequat metus sapien ut nunc.', 'Donec diam neque, vestibulum eget, vulputate ut, ultrices vel, augue.');
insert into huge_vocabulary (word, meaning, examples) values ('id', 'Duis consequat dui nec nisi volutpat eleifend. Donec ut dolor. Morbi vel lectus in quam fringilla rhoncus. Mauris enim leo, rhoncus sed, vestibulum sit amet, cursus id, turpis.', 'Donec ut dolor.');
insert into huge_vocabulary (word, meaning, examples) values ('lobortis', 'Nam dui. Proin leo odio, porttitor id, consequat in, consequat ut, nulla. Sed accumsan felis. Ut at dolor quis odio consequat varius.', 'Duis ac nibh.');
insert into huge_vocabulary (word, meaning, examples) values ('consectetuer', 'Curabitur convallis. Duis consequat dui nec nisi volutpat eleifend. Donec ut dolor. Morbi vel lectus in quam fringilla rhoncus.', 'Praesent lectus.');
insert into huge_vocabulary (word, meaning, examples) values ('quisque', 'Duis bibendum. Morbi non quam nec dui luctus rutrum. Nulla tellus. In sagittis dui vel nisl.', 'In hac habitasse platea dictumst.');
insert into huge_vocabulary (word, meaning, examples) values ('arcu', 'In blandit ultrices enim. Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Proin interdum mauris non ligula pellentesque ultrices. Phasellus id sapien in sapien iaculis congue.', 'Aenean sit amet justo.');
insert into huge_vocabulary (word, meaning, examples) values ('ipsum', 'Nunc purus. Phasellus in felis. Donec semper sapien a libero. Nam dui.', 'Nulla neque libero, convallis eget, eleifend luctus, ultricies eu, nibh.');
insert into huge_vocabulary (word, meaning, examples) values ('vitae', 'Donec posuere metus vitae ipsum. Aliquam non mauris. Morbi non lectus. Aliquam sit amet diam in magna bibendum imperdiet.', 'Lorem ipsum dolor sit amet, consectetuer adipiscing elit.');
insert into huge_vocabulary (word, meaning, examples) values ('rutrum', 'Morbi vestibulum, velit id pretium iaculis, diam erat fermentum justo, nec condimentum neque sapien placerat ante. Nulla justo. Aliquam quis turpis eget elit sodales scelerisque. Mauris sit amet eros.', 'Nullam sit amet turpis elementum ligula vehicula consequat.');
insert into huge_vocabulary (word, meaning, examples) values ('dapibus', 'Duis at velit eu est congue elementum. In hac habitasse platea dictumst. Morbi vestibulum, velit id pretium iaculis, diam erat fermentum justo, nec condimentum neque sapien placerat ante. Nulla justo.', 'Pellentesque ultrices mattis odio.');
insert into huge_vocabulary (word, meaning, examples) values ('phasellus', 'Vestibulum ac est lacinia nisi venenatis tristique. Fusce congue, diam id ornare imperdiet, sapien urna pretium nisl, ut volutpat sapien arcu sed augue. Aliquam erat volutpat. In congue.', 'Quisque ut erat.');
insert into huge_vocabulary (word, meaning, examples) values ('etiam', 'Mauris sit amet eros. Suspendisse accumsan tortor quis turpis. Sed ante. Vivamus tortor.', 'Lorem ipsum dolor sit amet, consectetuer adipiscing elit.');
insert into huge_vocabulary (word, meaning, examples) values ('commodo', 'Morbi porttitor lorem id ligula. Suspendisse ornare consequat lectus. In est risus, auctor sed, tristique in, tempus sit amet, sem. Fusce consequat.', 'Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Nulla dapibus dolor vel est.');
insert into huge_vocabulary (word, meaning, examples) values ('primis', 'Nullam varius. Nulla facilisi. Cras non velit nec nisi vulputate nonummy. Maecenas tincidunt lacus at velit.', 'Vivamus vel nulla eget eros elementum pellentesque.');
insert into huge_vocabulary (word, meaning, examples) values ('diam', 'Duis bibendum. Morbi non quam nec dui luctus rutrum. Nulla tellus. In sagittis dui vel nisl.', 'Nulla ut erat id mauris vulputate elementum.');
insert into huge_vocabulary (word, meaning, examples) values ('at', 'Nulla tempus. Vivamus in felis eu sapien cursus vestibulum. Proin eu mi. Nulla ac enim.', 'Phasellus in felis.');
insert into huge_vocabulary (word, meaning, examples) values ('proin', 'Aliquam non mauris. Morbi non lectus. Aliquam sit amet diam in magna bibendum imperdiet. Nullam orci pede, venenatis non, sodales sed, tincidunt eu, felis.', 'Proin leo odio, porttitor id, consequat in, consequat ut, nulla.');
insert into huge_vocabulary (word, meaning, examples) values ('justo', 'Nullam sit amet turpis elementum ligula vehicula consequat. Morbi a ipsum. Integer a nibh. In quis justo.', 'Maecenas tincidunt lacus at velit.');
insert into huge_vocabulary (word, meaning, examples) values ('praesent', 'Phasellus id sapien in sapien iaculis congue. Vivamus metus arcu, adipiscing molestie, hendrerit at, vulputate vitae, nisl. Aenean lectus. Pellentesque eget nunc.', 'Maecenas pulvinar lobortis est.');
insert into huge_vocabulary (word, meaning, examples) values ('sit', 'Nunc nisl. Duis bibendum, felis sed interdum venenatis, turpis enim blandit mi, in porttitor pede justo eu massa. Donec dapibus. Duis at velit eu est congue elementum.', 'Quisque ut erat.');
insert into huge_vocabulary (word, meaning, examples) values ('nunc', 'Suspendisse potenti. In eleifend quam a odio. In hac habitasse platea dictumst. Maecenas ut massa quis augue luctus tincidunt.', 'Suspendisse potenti.');
insert into huge_vocabulary (word, meaning, examples) values ('praesent', 'Nulla tellus. In sagittis dui vel nisl. Duis ac nibh. Fusce lacus purus, aliquet at, feugiat non, pretium quis, lectus.', 'Vivamus tortor.');
insert into huge_vocabulary (word, meaning, examples) values ('libero', 'Quisque porta volutpat erat. Quisque erat eros, viverra eget, congue eget, semper rutrum, nulla. Nunc purus. Phasellus in felis.', 'Cras mi pede, malesuada in, imperdiet et, commodo vulputate, justo.');
insert into huge_vocabulary (word, meaning, examples) values ('leo', 'Integer aliquet, massa id lobortis convallis, tortor risus dapibus augue, vel accumsan tellus nisi eu orci. Mauris lacinia sapien quis libero. Nullam sit amet turpis elementum ligula vehicula consequat. Morbi a ipsum.', 'Curabitur at ipsum ac tellus semper interdum.');
insert into huge_vocabulary (word, meaning, examples) values ('lorem', 'Vivamus metus arcu, adipiscing molestie, hendrerit at, vulputate vitae, nisl. Aenean lectus. Pellentesque eget nunc. Donec quis orci eget orci vehicula condimentum.', 'Aenean sit amet justo.');
insert into huge_vocabulary (word, meaning, examples) values ('sit', 'Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Proin interdum mauris non ligula pellentesque ultrices. Phasellus id sapien in sapien iaculis congue. Vivamus metus arcu, adipiscing molestie, hendrerit at, vulputate vitae, nisl.', 'Morbi ut odio.');
insert into huge_vocabulary (word, meaning, examples) values ('nulla', 'Integer aliquet, massa id lobortis convallis, tortor risus dapibus augue, vel accumsan tellus nisi eu orci. Mauris lacinia sapien quis libero. Nullam sit amet turpis elementum ligula vehicula consequat. Morbi a ipsum.', 'Morbi non quam nec dui luctus rutrum.');
insert into huge_vocabulary (word, meaning, examples) values ('elementum', 'Etiam justo. Etiam pretium iaculis justo. In hac habitasse platea dictumst. Etiam faucibus cursus urna.', 'Maecenas rhoncus aliquam lacus.');
insert into huge_vocabulary (word, meaning, examples) values ('turpis', 'Nullam varius. Nulla facilisi. Cras non velit nec nisi vulputate nonummy. Maecenas tincidunt lacus at velit.', 'Suspendisse accumsan tortor quis turpis.');
insert into huge_vocabulary (word, meaning, examples) values ('quam', 'Mauris lacinia sapien quis libero. Nullam sit amet turpis elementum ligula vehicula consequat. Morbi a ipsum. Integer a nibh.', 'In est risus, auctor sed, tristique in, tempus sit amet, sem.');
insert into huge_vocabulary (word, meaning, examples) values ('viverra', 'In hac habitasse platea dictumst. Morbi vestibulum, velit id pretium iaculis, diam erat fermentum justo, nec condimentum neque sapien placerat ante. Nulla justo. Aliquam quis turpis eget elit sodales scelerisque.', 'Sed ante.');
insert into huge_vocabulary (word, meaning, examples) values ('vivamus', 'Morbi vel lectus in quam fringilla rhoncus. Mauris enim leo, rhoncus sed, vestibulum sit amet, cursus id, turpis. Integer aliquet, massa id lobortis convallis, tortor risus dapibus augue, vel accumsan tellus nisi eu orci. Mauris lacinia sapien quis libero.', 'Fusce posuere felis sed lacus.');
insert into huge_vocabulary (word, meaning, examples) values ('ut', 'Quisque ut erat. Curabitur gravida nisi at nibh. In hac habitasse platea dictumst. Aliquam augue quam, sollicitudin vitae, consectetuer eget, rutrum at, lorem.', 'Ut at dolor quis odio consequat varius.');
insert into huge_vocabulary (word, meaning, examples) values ('felis', 'Nam congue, risus semper porta volutpat, quam pede lobortis ligula, sit amet eleifend pede libero quis orci. Nullam molestie nibh in lectus. Pellentesque at nulla. Suspendisse potenti.', 'Aenean sit amet justo.');
insert into huge_vocabulary (word, meaning, examples) values ('quam', 'Aliquam erat volutpat. In congue. Etiam justo. Etiam pretium iaculis justo.', 'Donec diam neque, vestibulum eget, vulputate ut, ultrices vel, augue.');
insert into huge_vocabulary (word, meaning, examples) values ('donec', 'Suspendisse potenti. Nullam porttitor lacus at turpis. Donec posuere metus vitae ipsum. Aliquam non mauris.', 'Duis aliquam convallis nunc.');
insert into huge_vocabulary (word, meaning, examples) values ('cras', 'Sed sagittis. Nam congue, risus semper porta volutpat, quam pede lobortis ligula, sit amet eleifend pede libero quis orci. Nullam molestie nibh in lectus. Pellentesque at nulla.', 'Sed vel enim sit amet nunc viverra dapibus.');
insert into huge_vocabulary (word, meaning, examples) values ('rutrum', 'Mauris lacinia sapien quis libero. Nullam sit amet turpis elementum ligula vehicula consequat. Morbi a ipsum. Integer a nibh.', 'Suspendisse potenti.');
insert into huge_vocabulary (word, meaning, examples) values ('quis', 'Maecenas tristique, est et tempus semper, est quam pharetra magna, ac consequat metus sapien ut nunc. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Mauris viverra diam vitae quam. Suspendisse potenti. Nullam porttitor lacus at turpis.', 'Duis mattis egestas metus.');
insert into huge_vocabulary (word, meaning, examples) values ('nam', 'Nullam varius. Nulla facilisi. Cras non velit nec nisi vulputate nonummy. Maecenas tincidunt lacus at velit.', 'Donec dapibus.');
insert into huge_vocabulary (word, meaning, examples) values ('odio', 'Vivamus vestibulum sagittis sapien. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Etiam vel augue. Vestibulum rutrum rutrum neque.', 'Suspendisse potenti.');
insert into huge_vocabulary (word, meaning, examples) values ('non', 'Morbi quis tortor id nulla ultrices aliquet. Maecenas leo odio, condimentum id, luctus nec, molestie sed, justo. Pellentesque viverra pede ac diam. Cras pellentesque volutpat dui.', 'Donec quis orci eget orci vehicula condimentum.');
)sql");
    }
}
