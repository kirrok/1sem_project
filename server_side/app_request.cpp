//
// Created by kirrok on 01.05.16.
//

#include <cstring>
#include "app_request.h"


std::string &app_request::build_query_one(int lt, int ut, int activities_number, int *activities) {
    query = "SELECT place_id, place_name, place_weather from appDB.place WHERE place.place_weather BETWEEN ";
    query += std::to_string(lt);
    query += " AND ";
    query += std::to_string(ut);
    query += " AND place_activity IN(";
    for (size_t i = 0; i < activities_number; i++) {

        if (i != activities_number - 1)
            query += std::to_string(activities[i]) + ",";
        else
            query += std::to_string(activities[i]);
    }
    query += ");";
    return query;
}

std::string &app_request::build_query_two(int id) {
    query = "SELECT descr_about, descr_logo, descr_activity, descr_language, descr_food, descr_language, descr_money from appDB.place_description WHERE place_description.descr_place = ";
    query += std::to_string(id);
    query += ";";
    return query;
}