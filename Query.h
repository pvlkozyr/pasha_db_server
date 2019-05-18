#ifndef PASHA_DB_SERVER_QUERY_H
#define PASHA_DB_SERVER_QUERY_H

#include <string>
#include <Record.h>

class Range {
private:
    int min;
    int max;
public:
    Range();
    bool check(int value);

    explicit Range(std::string);

    ~Range();
};

class IpRange {
private:
    int o1;
    int o2;
    int o3;
    int o4;
public:
    IpRange();

    bool check(Ip);

    explicit IpRange(std::string);

    ~IpRange();
};

class DateRange {
private:
    int year;
    int month;
    int day;
public:
    bool check(Date);

    DateRange();
    explicit DateRange(std::string);
    ~DateRange();
};

class FioRange {
private:
    std::string firstName;
    std::string lastName;
    std::string patronimic;
public:
    bool check(Fio);

    FioRange();
    explicit FioRange(std::string, std::string, std::string);
    ~FioRange();
};

class Query {
private:
    Range id;
    IpRange ip;
    DateRange date;
    FioRange fio;
    std::string read_field(std::string field_name, std::string query);
public:
    Query();

    explicit Query(std::string);

    ~Query();

    bool check(Record);
};


#endif //PASHA_DB_SERVER_QUERY_H
