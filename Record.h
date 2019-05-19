//
// Created by user on 5/18/2019.
//

#ifndef PASHA_DB_SERVER_RECORD_H
#define PASHA_DB_SERVER_RECORD_H

#include <vector>
#include <string>

struct Fio{
    std::string firstName;
    std::string lastName;
    std::string patronimic;
    std::string print()
    {
        return firstName + " " + lastName + " " + patronimic;
    }
};
struct Ip{
    int o1;
    int o2;
    int o3;
    int o4;
    std::string print()
    {
        return std::to_string(o1) + "." + std::to_string(o2) + "." + std::to_string(o3) + "." + std::to_string(o4);
    }
};
struct Date{
    int year;
    int month;
    int day;
    std::string print()
    {
        return std::to_string(year) + "-" +std::to_string(month) + "-" +std::to_string(day);
    }

};

struct Record {
    int id;
    Fio fio;
    Ip ip;
    Date date;
    std::vector<int> trafficIn;
    std::vector<int> trafficOut;
    std::string print()
    {
        return fio.print() + "\t" + ip.print() + "\t" + date.print()+"\n";
    }
};

#endif //PASHA_DB_SERVER_RECORD_H
