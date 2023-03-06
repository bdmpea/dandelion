#ifndef DANDELION_DATABASE_H
#define DANDELION_DATABASE_H

namespace Dandelion::Server {

struct Database {
public:

private:
    friend struct UserStorage;
    friend struct DictionariesStorage;
};

struct UserStorage {
private:
    Database &db;
};

struct DictionariesStorage {
private:
    Database &db;
};


}  // namespace Dandelion::Server

#endif  // DANDELION_DATABASE_H
