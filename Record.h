//
// Created by user on 5/18/2019.
//

#ifndef PASHA_DB_SERVER_RECORD_H
#define PASHA_DB_SERVER_RECORD_H

#include <vector>
#include <string>

struct Fio{
    const std::string firstName;
    const std::string lastName;
    const std::string patronimic;
};
struct Ip{
    const int o1;
    const int o2;
    const int o3;
    const int o4;
};
struct Date{
    const int year;
    const int month;
    const int day;
};

struct Record {
    int id;
    Fio fio;
    Ip ip;
    Date date;
    std::vector<int> trafficIn;
    std::vector<int> trafficOut;
};

#endif //PASHA_DB_SERVER_RECORD_H
