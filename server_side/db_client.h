//
// Created by kirrok on 01.05.16.
//

#ifndef SERVER_SIDE_DB_CLIENT_H
#define SERVER_SIDE_DB_CLIENT_H

#include <mysql/mysql.h>
#include <string>


class db_client {
public:
    db_client(const char *name_ = "appDB", const char *user_ = "root",
              const char *host_ = "localhost", const char *password_ = "1", const char *socket_ = NULL,
              const unsigned int port_ = 3306) :
            name(name_), user(user_), host(host_), password(password_), socket(socket_),
            port(port_) { };


    unsigned long query_(std::string &query);


    MYSQL_RES *get_res() {
        return res;
    }

private:
    MYSQL_RES *res;
    const char *host, *name, *user, *password, *socket;
    const unsigned int port;
};


#endif //SERVER_SIDE_DB_CLIENT_H
