#include <iostream>
#include <json/json_object.h>
#include "jconverter.h"
#include "app_request.h"
#include "db_client.h"


#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <memory>

using namespace std;

void error(const char *msg) {
    cout << msg << endl;
    exit(1);
}

const int portno = 5002;

int main() {
    int sockfd, newsockfd;
    socklen_t clilent_addr_size;
    int *header = new int;

    sockaddr_in serv_addr, cli_addr;

    ssize_t n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    int t = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &t, sizeof(int));
    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd, 5);

    cout << "СЛУШАЮ!" << endl;


    while (1) {

        clilent_addr_size = sizeof(cli_addr);
        newsockfd = accept(sockfd, (sockaddr *) &cli_addr, &clilent_addr_size);

        if (newsockfd < 0)
            error("ERROR on accept");

        n = read(newsockfd, header, sizeof(int));
        if (n < 0) error("ERROR reading to socket");

        char *buffer = new char[*header];
        bzero(buffer, *header);
        cout <<"11111111:" << buffer << endl;
        cout << "Получили от клиента длину: " << *header << endl;

        n = read(newsockfd, buffer, *header);
        cout << buffer << endl;
        if (n < 0) error("ERROR reading to socket");
        cout << "\nПолучили от клиента строку:" << buffer <<endl;

        json_object *jquery = json_tokener_parse(buffer);

        cout << " \nКонвертируем в json: " << json_object_to_json_string(jquery) << endl;

        jconverter converter(jquery);

        app_request ar;
        db_client db;
        json_object *response;

        if (converter.type() == 1) {
            data data_ = converter.convert_json_to_data_one();

            string query = ar.build_query_one(data_.lt, data_.ut, data_.act_number, data_.act);
            cout << "\nЗапрос который пойдет в базу:" << query << endl;

            unsigned long rows_number = db.query_(query);
            MYSQL_RES *res = db.get_res();

            response = converter.convert_db_response_to_json_one(res, rows_number);
            cout << "\n Json - ответ:  "<< json_object_to_json_string(response) << endl;

        } else if (converter.type() == 2) {
            int id = converter.convert_json_to_data_two();
            string query = ar.build_query_two(id);
            cout << "\nЗапрос который пойдет в базу:" << query << endl;

            unsigned long rows_number = db.query_(query);

            MYSQL_RES *res = db.get_res();

            response = converter.convert_db_response_to_json_two(res);

            cout << "\n Json - ответ: " << flush << json_object_to_json_string(response) << endl;

        } else {
            cout << "Incorrect request type!" << endl;
            continue;
        }

        *header = strlen(json_object_to_json_string(response));

        n = write(newsockfd, header, sizeof(int));
        if (n < 0) error("ERROR reading to socket");

        n = write(newsockfd, json_object_to_json_string(response), *header);
        if (n < 0) error("ERROR reading to socket");

        delete[] buffer;

        close(newsockfd);
    }
    close(sockfd);

    return 0;
}