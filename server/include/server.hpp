#ifndef UNTITLED2_SERVER_HPP
#define UNTITLED2_SERVER_HPP
#include <iostream>
#include "string"
namespace Dandelion::Server {
    struct server{
    public:
        bool register_user(const std::string &login,const std::string &password);
        bool check_signing_in(const std::string &login,const std::string &password);
        static server &getInstance(){
            static server Server;
            return Server;
        }

    };
    struct model_changes{

    };
}

#endif //UNTITLED2_SERVER_HPP
