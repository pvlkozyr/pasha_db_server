//
// Created by user on 5/18/2019.
//

#include "Query.h"
#include "Record.h"

std::string Query::read_field(std::string field_name, std::string query) {
    return std::__cxx11::string();
}

Query::Query(std::string query) {
    this->id = Range(read_field("id", query));
    //lastName=Иванов
    //firstName=Иван
    //patronimic=Иванович
    this->fio = FioRange(
            read_field("firstName", query),
            read_field("lastName", query),
            read_field("patronimic", query)
    );
    //ip=123.168.*.*
    this->ip = IpRange(read_field("ip", query));
    //ip=123.168.*.*
    this->date = DateRange(read_field("date", query));
}

Query::~Query() {

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

Range::~Range() {

}
