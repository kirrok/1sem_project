//
// Created by kirrok on 01.05.16.
//

#ifndef SERVER_SIDE_APP_REQUEST_H
#define SERVER_SIDE_APP_REQUEST_H


#include <string>

class app_request {
public:
    std::string &build_query_one(int lt_, int ut_, int anum, int *activities_);

    std::string &build_query_two(int id);

private:
    std::string query;
};


#endif //SERVER_SIDE_APP_REQUEST_H
