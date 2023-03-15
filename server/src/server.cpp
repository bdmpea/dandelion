#include "server.hpp"
namespace Dandelion::Server{
    bool server::register_user(const std::string &login, const std::string &password){
        try {
          //  is_used_login(login);    use function hear to connect with BD
        }catch(std::exception &exp){
            return false;
        }
        //create_user(login, password);
        return true;
    }
    bool server::check_signing_in(const std::string &login,const std::string &password){
        try {
          //  is_correct_account_login(login, password);   use function hear to connect with BD
        }catch(std::exception &exp){
            return false;
        }
        return true;
    }
}
