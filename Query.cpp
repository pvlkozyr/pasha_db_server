//
// Created by user on 5/18/2019.
//

#include "Query.h"
#include "Record.h"
//#include<iostream>
Range::Range()
{
    max = -1;
    min = -1;
}

bool Range::check(int value) {
    if (max == -1 && min == -1)
        return true;
    else if (max == -1)
        return value == min;
    else
        return value >= min && value <= max;
}

//id=1234-2345
Range::Range(std::string str_range) {
    max = -1;
    min = -1;
    if (str_range.size() == 0) return;
    unsigned int pos = str_range.find('-');
    if (pos != std::string::npos) {
        min = std::stoi(str_range.substr(0, pos));
        max = std::stoi(str_range.substr(pos + 1));
    } else {
        min = std::stoi(str_range);
    }
}

IpRange::IpRange()
{
    o1 = -1;
    o2 = -1;
    o3 = -1;
    o4 = -1;
}
bool IpRange::check(Ip ip) {
    if (o1 == -1 && o2 == -1 && o3 == -1 && o4 == -1)
        return true;
    else if (o1 == ip.o1 && o2 == ip.o2 && o3 == ip.o3 && o4 == -1)
        return true;
    else if (o1 == ip.o1 && o2 == ip.o2 && o3 == -1 && o4 == -1)
        return true;
    else if (o1 == ip.o1 && o2 == -1 && o3 == -1 && o4 == -1 )
        return true;
    else if (o1 == ip.o1 && o2 == ip.o2 && o3 == ip.o3 && o4 == ip.o4)
        return true;
    else return false;
}

IpRange::IpRange(std::string str_range)
{
    o1 = -1;
    o2 = -1;
    o3 = -1;
    o4 = -1;
    if (str_range.size() == 0) return;
    unsigned int pos = str_range.find('.');
    o1 = std::stoi(str_range.substr(0,pos));
    unsigned int pos1 = str_range.find('.',pos+1);
    o2 = std::stoi(str_range.substr(pos,pos1));
    pos = str_range.find('.',pos1+1);
    o3 = std::stoi(str_range.substr(pos1,pos));
    o4 = std::stoi(str_range.substr(pos+1));
}
DateRange::DateRange()
{
    year = -1;
    month = -1;
    day = -1;
}
bool DateRange::check(Date date)
{
    if (year == -1 && month == -1 && day == -1)
        return true;
    else if (year == date.year && month == date.month && day == -1)
        return true;
    else if (year == date.year && month == -1 && day == -1)
        return true;
    else if (year == date.year && month == date.month && day == date.day)
        return true;
    else return false;
}

DateRange::DateRange(std::string str_range)
{
    year = -1;
    month = -1;
    day = -1;
    if (str_range.size() == 0) return;
    int pos = str_range.find('-');
    if (pos != std::string::npos) {
        year = std::stoi(str_range.substr(0, pos));
    } else {
        year = std::stoi(str_range);
        return;
    }
    int pos1 = str_range.find('-',pos+1);
    if (pos1 != std::string::npos) {
        month = std::stoi(str_range.substr(pos, pos1));
    } else {
        month = std::stoi(str_range.substr(pos));
        return;
    }
    day = std::stoi(str_range.substr(pos1+1));
}

FioRange::FioRange()
{
    firstName = "";
    lastName = "";
    patronimic = "";
}
bool FioRange::check(Fio fio)
{
    if (firstName == "" && lastName == "" && patronimic == "")
        return true;
    else if ((firstName == "" && lastName == "" && patronimic == fio.patronimic)||
             (firstName == "" && lastName == fio.lastName && patronimic == "")||
             (firstName == fio.firstName && lastName == "" && patronimic == ""))
        return true;
    else if (firstName == fio.firstName && lastName == fio.lastName && patronimic == fio.patronimic)\
        return true;
    else return false;
}
FioRange::FioRange(std::string firstName,std::string lastName,std::string patronimic)
{
    this->firstName = firstName;
    this->lastName = lastName;
    this->patronimic = patronimic;
}

std::string Query::read_field(std::string field_name, std::string query) {
    int pos = query.find(field_name);
//    std::cout << field_name << " query:" << query << std::endl;
    if(pos != std::string::npos) {
        int pos1 = query.find(':', pos + 1);
        unsigned int value_index = pos + field_name.length();
        if(pos1 != std::string::npos) {
//            std::cout <<query.substr(value_index, pos1 - value_index )<<std::endl;
            return query.substr(value_index, pos1 - value_index );
        } else {
//            std::cout <<query.substr(value_index)<<std::endl;
            return query.substr(value_index);
        }
    } else {
        return "";
    }
}

Query::Query(std::string query) {
    this->style = read_field("style=", query);
    this->id = Range(read_field("id=", query));
    //lastName=Иванов
    //firstName=Иван
    //patronimic=Иванович
    this->fio = FioRange(
            read_field("firstName=", query),
            read_field("lastName=", query),
            read_field("patronimic=", query)
    );
    //ip=123.168.*.*
    this->ip = IpRange(read_field("ip=", query));
    //ip=123.168.*.*
    this->date = DateRange(read_field("date=", query));
}
bool Query::check(Record record) {
    return this->id.check(record.id)
           &&
           this->fio.check(record.fio)
           &&
           this->ip.check(record.ip)
           &&
           this->date.check(record.date);
}
